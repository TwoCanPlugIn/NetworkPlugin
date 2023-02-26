// Copyright(C) 2023 by Steven Adler
//
// This file is part of NMEA 2000 Network plugin for OpenCPN.
//
// NMEA 2000 Network plugin for OpenCPN is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// NMEA 2000 Network plugin for OpenCPN is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with the NMEA 2000 Network plugin for OpenCPN. If not, see <https://www.gnu.org/licenses/>.
//
// NMEA2000® is a registered trademark of the National Marine Electronics Association

//
// Project: NMEA 2000 Network Plugin
// Description: NMEA 2000 Network display for OpenCPN
// Owner: twocanplugin@hotmail.com
// Date: 31/01/2023
// Version History: 
// 1.0 Initial Release

#include "network_pi_plugin.h"

#include "network_pi_icons.h"

//  Debug spew via UDP
wxDatagramSocket *udpSocket;
wxIPV4address addrLocal;
wxIPV4address addrPeer;

// The class factories, used to create and destroy instances of the PlugIn
extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr) {
	return new NetworkPlugin(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p) {
	delete p;
}

// Constructor
NetworkPlugin::NetworkPlugin(void *ppimgr) : opencpn_plugin_118(ppimgr), wxEvtHandler() {
	
	// Load the plugin bitmaps/icons 
	initialize_images();

	// Initialize Advanced User Interface Manager (AUI)
	auiManager = GetFrameAuiManager();
	auiManager->Connect(wxEVT_AUI_PANE_CLOSE, wxAuiManagerEventHandler(NetworkPlugin::OnPaneClose), NULL, this);
}

// Destructor
NetworkPlugin::~NetworkPlugin(void) {
	auiManager->Disconnect(wxEVT_AUI_PANE_CLOSE, wxAuiManagerEventHandler(NetworkPlugin::OnPaneClose), NULL, this);
}

int NetworkPlugin::Init(void) {
	// Maintain a reference to the OpenCPN window to use as the parent for the dialog
	parentWindow = GetOCPNCanvasWindow();

	// Maintain a reference to the OpenCPN configuration object 
	configSettings = GetOCPNConfigObject();

	// Load the configuration settings
	if (configSettings) {
		configSettings->SetPath(_T("/PlugIns/Network"));
		configSettings->Read(_T("Visible"), &isNetworkDialogVisible, FALSE);
		configSettings->Read(_T("Interval"), &heartbeatInterval, 0);
		configSettings->Read(_T("Interface"), &interfaceName, wxEmptyString);
		configSettings->Read(_T("Heartbeat"), &sendHeartbeat, FALSE);
		configSettings->Read(_T("Request"), &sendRequest, FALSE);
	}

	// Load toolbar icons
	wxString shareLocn = GetPluginDataDir(PLUGIN_PACKAGE_NAME) + wxFileName::GetPathSeparator() + _T("data") + wxFileName::GetPathSeparator();

	wxString normalIcon = shareLocn + _T("network-normal.svg");
	wxString toggledIcon = shareLocn + _T("network-toggled.svg");
	wxString rolloverIcon = shareLocn + _T("network-rollover.svg");

	// Initialize the toolbar
	networkToolbar = InsertPlugInToolSVG(_T(""), normalIcon, rolloverIcon, toggledIcon, wxITEM_CHECK,_("NMEA 2000 Network"), _T("Display devices on NMEA 2000 Network"), NULL, -1, 0, this);

	

	// Need to notify Actisense NGT-1 what PGN's we transmit
	// We only transmit PGN 59904 ISO Request
	// Guessing that YachtDevices & socketCan ignore this (or at least NOP)
	std::vector<int> pgnList{ 59904 };
	CommDriverResult result;
	//result = RegisterTXPGNs(driverHandle, pgnList);

	// Initialize NMEA 2000 Listeners

	// PGN 60928 ISO Address Claim
	wxDEFINE_EVENT(EVT_N2K_60928, ObservedEvt);
	NMEA2000Id id_60928 = NMEA2000Id(60928);
	listener_60928 = std::move(GetListener(id_60928, EVT_N2K_60928, this));
	Bind(EVT_N2K_60928, [&](ObservedEvt ev) {
		HandleN2K_60928(ev);
	});

	// PGN 126996 Product Information
	wxDEFINE_EVENT(EVT_N2K_126996, ObservedEvt);
	NMEA2000Id id_126996 = NMEA2000Id(126996);
	listener_126996 = std::move(GetListener(id_126996, EVT_N2K_126996, this));
	Bind(EVT_N2K_126996, [&](ObservedEvt ev) {
		HandleN2K_126996(ev);
	});

	// PGN 126998 Configuration Information
	wxDEFINE_EVENT(EVT_N2K_126998, ObservedEvt);
	NMEA2000Id id_126998 = NMEA2000Id(126998);
	listener_126998 = std::move(GetListener(id_126998, EVT_N2K_126998, this));
	Bind(EVT_N2K_126998, [&](ObservedEvt ev) {
		HandleN2K_126998(ev);
	});

	// PGN 126993 NMEA Heartbeat
	wxDEFINE_EVENT(EVT_N2K_126993, ObservedEvt);
	NMEA2000Id id_126993 = NMEA2000Id(126993);
	listener_126993 = std::move(GetListener(id_126993, EVT_N2K_126993, this));
	Bind(EVT_N2K_126993, [&](ObservedEvt ev) {
		HandleN2K_126993(ev);
	});

	// PGN 126464 Supported PGN
	wxDEFINE_EVENT(EVT_N2K_126464, ObservedEvt);
	NMEA2000Id id_126464 = NMEA2000Id(126464);
	listener_126464 = std::move(GetListener(id_126464, EVT_N2K_126464, this));
	Bind(EVT_N2K_126464, [&](ObservedEvt ev) {
		HandleN2K_126464(ev);
	});

	// Example of adding context menu items including separators etc.
	// BUG BUG Remove ??
	wxMenuItem *myMenu = new wxMenuItem(NULL, wxID_HIGHEST + 1, _T("NMEA 2000"), wxEmptyString, wxITEM_NORMAL, NULL);
	networkContextMenu = AddCanvasContextMenuItem(myMenu, this);

	// Wire up the Network Dialog Close event
	Connect(wxEVT_NETWORK_PLUGIN_EVENT, wxCommandEventHandler(NetworkPlugin::OnPluginEvent));

	// Instantiate the dialog
	networkDialog = new  NetworkDialog(parentWindow, this);

	// Load our dialog into the AUI Manager
	paneInfo.Name(_T("Network Plugin"));
	paneInfo.Caption("NMEA 2000 Network");
	paneInfo.Float();
	paneInfo.Hide();
	paneInfo.Dockable(FALSE);
	auiManager->AddPane(networkDialog, paneInfo);
	auiManager->Update();

	// BUG BUG Superfluous
	if (paneInfo.IsShown()) {
		isNetworkDialogVisible = TRUE;
	}
	else {
		isNetworkDialogVisible = FALSE;
	}

	// Synchronize toolbar status
	SetToolbarItemState(networkToolbar, isNetworkDialogVisible);
	
	// OpenCPN Settings Dialog toolbox extension
	toolboxPanel = nullptr;

	// Protect against reading the network list being updaed
	// BUG BUG Is this used anywhere ??
	lockNetworkData = new wxCriticalSection();
	
	// BUG BUG Investigating some different API's
	// BUG BUG Remove
	wxLogMessage(_T("*** Distance Unit: %s"),getUsrDistanceUnit_Plugin(-1));
	wxLogMessage(_T("*** Speed Unit: %s"), getUsrSpeedUnit_Plugin(-1));
	wxLogMessage(_T("*** Shared: %s"), *GetpSharedDataLocation());
	wxLogMessage(_T("*** Exe Path: %s"), GetOCPN_ExePath());
	wxLogMessage(_T("*** Plugin Location: %s"), *GetpPlugInLocation());
	wxLogMessage(_T("*** Private App: %s"), *GetpPrivateApplicationDataLocation());
	wxLogMessage(_T("*** Doc Path: %s"), GetWritableDocumentsDir());
	wxLogMessage(_T("*** Toolbox: %d"), GetToolboxPanelCount());
	wxLogMessage(_T("*** Data Dir: %s"), GetPluginDataDir("network_pi")); 
	// BUG BUG GetPluginDataDir doesn't work the way I wold expect it to.
	// It won't use the Plugin Common Name "Network Plugin", which is what I would have expected 

	// Initialize Debug Spew via UDP
	addrLocal.Hostname();
	addrPeer.Hostname("127.0.0.1");
	addrPeer.Service(3001);

	udpSocket = new wxDatagramSocket(addrLocal, wxSOCKET_NONE);
	if (!udpSocket->IsOk()) {
		wxLogMessage(_T("ERROR: failed to create UDP peer socket"));
	}

	// Start a timer to transmit NMEA 2000 network queries
	if (sendHeartbeat) {
		heartbeatTimer = new wxTimer();
		heartbeatTimer->Connect(heartbeatTimer->GetId(), wxEVT_TIMER, wxTimerEventHandler(NetworkPlugin::OnTimer), NULL, this);
		heartbeatTimer->Start(heartbeatInterval * 60000);
	}

	// Notify OpenCPN of our capabilities and what events we want to receive callbacks for
	return (WANTS_CONFIG | WANTS_TOOLBAR_CALLBACK | INSTALLS_TOOLBAR_TOOL | INSTALLS_CONTEXTMENU_ITEMS | USES_AUI_MANAGER | WANTS_CURSOR_LATLON | WANTS_OVERLAY_CALLBACK | INSTALLS_TOOLBOX_PAGE | WANTS_NMEA_EVENTS | WANTS_NMEA_SENTENCES | WANTS_AIS_SENTENCES | WANTS_LATE_INIT | WANTS_PLUGIN_MESSAGING);
}

// Timer sends heartbeat and network request PGN's
void NetworkPlugin::OnTimer(wxTimerEvent &event) {

	wxLogMessage(_T("Network Plugin - On Timer"));
	
	CommDriverResult result;

	std::vector<uint8_t> payload;

	payload.push_back(60928 & 0xFF);
	payload.push_back((60928 >> 8) & 0xFF);
	payload.push_back((60928 >> 16) & 0xFF);

	auto sharedPointer = std::make_shared<std::vector<uint8_t> >(std::move(payload));

	//result = WriteCommDriverN2K(driverHandle, 59904, 255, 5, sharedPointer);

	payload.clear();
	payload.push_back(126996 & 0xFF);
	payload.push_back((126996 >> 8) & 0xFF);
	payload.push_back((126996 >> 16) & 0xFF);

	sharedPointer = std::make_shared<std::vector<uint8_t> >(std::move(payload));

	//result = WriteCommDriverN2K(driverHandle, 59904, 255, 5, sharedPointer);

	payload.clear();
	payload.push_back(126998 & 0xFF);
	payload.push_back((126998 >> 8) & 0xFF);
	payload.push_back((126998 >> 16) & 0xFF);

	sharedPointer = std::make_shared<std::vector<uint8_t> >(std::move(payload));

	//result = WriteCommDriverN2K(driverHandle, 59904, 255, 5, sharedPointer);

}

void NetworkPlugin::OnPaneClose(wxAuiManagerEvent& event) {
	wxMessageBox(wxString::Format("plugin.cpp, OnPaneClose: %d", event.GetId()));

	isNetworkDialogVisible = FALSE;
	SetToolbarItemState(networkToolbar, isNetworkDialogVisible);
}

// OpenCPN is either closing down, or has been disabled from the Preferences Dialog
bool NetworkPlugin::DeInit(void) {
	// Cleanup Dialog Event Handler
	Disconnect(wxEVT_NETWORK_PLUGIN_EVENT, wxCommandEventHandler(NetworkPlugin::OnPluginEvent));

	// Save configuration settings
	if (configSettings) {
		configSettings->SetPath(_T("/PlugIns/Network"));
		configSettings->Write(_T("Visible"), isNetworkDialogVisible);
		configSettings->Write(_T("Interval"), heartbeatInterval);
		configSettings->Write(_T("Interface"), interfaceName);
		configSettings->Write(_T("Heartbeat"), sendHeartbeat);
		configSettings->Write(_T("Request"), sendRequest);
	}

	// Stop the timer
	if (sendHeartbeat) {
		heartbeatTimer->Stop();
		heartbeatTimer->Disconnect(wxEVT_TIMER, wxTimerEventHandler(NetworkPlugin::OnTimer), NULL, this);
		delete heartbeatTimer;
	}

	// Cleanup the critical section lock
	// BUG BUG Used anywhere ??
	delete lockNetworkData;

	// Cleanup Debug Spew
	udpSocket->Close();

	// Cleanup AUI
	auiManager->UnInit();
	auiManager->DetachPane(networkDialog);
	delete networkDialog;

	return TRUE;
}

void NetworkPlugin::UpdateAuiStatus(void) {
	wxMessageBox(wxString::Format("network plugin, UpdateAuiStatus: IsOK: %d IsShown: %d", paneInfo.IsOk(), paneInfo.IsShown()));
	
	if ((paneInfo.IsOk()) && (paneInfo.IsShown())) {
		isNetworkDialogVisible = TRUE;
		SetToolbarItemState(networkToolbar, isNetworkDialogVisible);
	}
}

void NetworkPlugin::LateInit(void) {
	// BUG BUG Can be removed
	std::string foobar = "Late Init";
	udpSocket->SendTo(addrPeer, foobar.data(), foobar.length());
}

// Indicate what version of the OpenCPN Plugin API we support
int NetworkPlugin::GetAPIVersionMajor() {
	return OCPN_API_VERSION_MAJOR;
}

int NetworkPlugin::GetAPIVersionMinor() {
	return OCPN_API_VERSION_MINOR;
}

// Our plugin version numbers. 
int NetworkPlugin::GetPlugInVersionMajor() {
	return PLUGIN_VERSION_MAJOR;
}

int NetworkPlugin::GetPlugInVersionMinor() {
	return PLUGIN_VERSION_MINOR;
}

// Return descriptions for the Plugin
wxString NetworkPlugin::GetCommonName() {
	return _T("Network Plugin");
}

wxString NetworkPlugin::GetShortDescription() {
	return _T("Network Plugin, Displays NMEA 2000 Network");
}

wxString NetworkPlugin::GetLongDescription() {
	return _T("Network Plugin, Displays information about the devices on the NMEA 2000 Network");
}

// 32x32 pixel image displayed in the Toolbox Plugins page
// Use pgn2wx.pl perl script to generate
wxBitmap* NetworkPlugin::GetPlugInBitmap() {
	return _img_network_colour;
}

// Add our own settings panel on the OpenCPN toolbox
// Requires INSTALLS_TOOLBOX_PAGE
void NetworkPlugin::OnSetupOptions(void) {
	// Get a handle to our options page window: ui settings, and add a sizer to it, to which we will add our toolbox panel
	wxScrolledWindow *optionsWindow = AddOptionsPage(OptionsParentPI::PI_OPTIONS_PARENT_UI, _T("Network"));
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	optionsWindow->SetSizer(sizer);
	// Create our toolbox panel and add it to the toolbox via the sizer
	toolboxPanel = new NetworkToolbox(optionsWindow);
	sizer->Add(toolboxPanel, 1, wxALL | wxEXPAND);
}

// I have no idea when this is called
void NetworkPlugin::SetupToolboxPanel(int page_sel, wxNotebook* pnotebook) {
	wxMessageBox(wxString::Format(_T("SetupToolboxPanel: %d"), page_sel));
}

// Invoked when the OpenCPN Toolbox OK, Apply or Cancel buttons are pressed
// Requires INSTALLS_TOOLBOX_PAGE
void NetworkPlugin::OnCloseToolboxPanel(int page_sel, int ok_apply_cancel) {
	if (((ok_apply_cancel == 0) || (ok_apply_cancel == 4)) && (settingsDirty == TRUE)) {
		// Save the settings
		if (configSettings) {
			configSettings->SetPath(_T("/PlugIns/Network"));
			configSettings->Write(_T("Visible"), isNetworkDialogVisible);
			configSettings->Write(_T("Interval"), heartbeatInterval);
			configSettings->Write(_T("Interface"), interfaceName);
			configSettings->Write(_T("Heartbeat"), sendHeartbeat);
			configSettings->Write(_T("Request"), sendRequest);
		}
	}
	delete toolboxPanel;
	toolboxPanel = nullptr;
}

// Return the number of toolbar icons the plugin installs
int NetworkPlugin::GetToolbarToolCount(void) {
 return 1;
}

// Invoked whenever the plugin is enabled from the toolbox
// Requires WANTS_CONFIG
void NetworkPlugin::SetDefaults(void) {
	// Is called when the plugin is enabled
	wxLogMessage(_T("*** Set Defaults called"));
}

// Invoked whenever a context menu item is selected
// Requires INSTALLS_CONTEXTMENU_ITEMS
// BUG BUG Remove as not useful
void NetworkPlugin::OnContextMenuItemCallback(int id) {
	if (id == networkContextMenu) {	
		wxMessageBox(wxString::Format(_T("Chart Tilt: %f"), GetCanvasTilt()));
	}
}

// Invoked by OpenCPN whenever a toolbar item is selected
// Requires WANTS_TOOLBAR_CALLBACK
void NetworkPlugin::OnToolbarToolCallback(int id) {
	// Toggles the display of the network dialog
	if (id == networkToolbar) {
		isNetworkDialogVisible = !isNetworkDialogVisible;
		auiManager->GetPane(_T("Network Plugin")).Show(isNetworkDialogVisible);
		auiManager->Update();
		SetToolbarItemState(id, isNetworkDialogVisible);
	}
}

// Handle events from the dialog
// BUG BUG Any events to handle ??
void NetworkPlugin::OnPluginEvent(wxCommandEvent &event) {
	switch (event.GetId()) {
		case NETWORKDIALOG_OPEN_EVENT:
			// Synchronises the state of the toolbar icon
			SetToolbarItemState(networkToolbar, isNetworkDialogVisible);
			break;
		case NETWORKDIALOG_CLOSE_EVENT:
			// Synchronises the state of the toolbar icon
			SetToolbarItemState(networkToolbar, isNetworkDialogVisible);
			break;
		case NETWORKDIALOG_PING_EVENT:
			break;
		default:
			event.Skip();
	}
}

// Raw NMEA 2000 generated by OpenCPN v5.8
// Parsing routines cut and pasted from TwoCan Plugin
// Refer to twocandevice.cpp

// Note the payload is not "the payload" but an entire Actisense payload
// Actisense application data, from NGT-1 to PC
// <data code=93><length (1)><priority (1)><PGN (3)><destination(1)><source
// (1)><time (4)><len (1)><data (len)>

// As applied to a real application data element, after extraction from packet
// format: 93 13 02 01 F8 01 FF 01 76 C2 52 00 08 08 70 EB 14 E8 8E 52 D2 BB 10

// data code		0x93
// length (1):      0x13
// priority (1);    0x02
// PGN (3):         0x01 0xF8 0x01
// destination(1):  0xFF
// source (1):      0x01
// time (4):        0x76 0xC2 0x52 0x00
// len (1):         0x08
// data (len):      08 70 EB 14 E8 8E 52 D2
// packet CRC:      0xBB

// To simplify parsing as these are copied from twocan plugin, 
// use an index into the "real" payload at byte 13 

// PGN 60928 ISO Address Claim
void NetworkPlugin::HandleN2K_60928(ObservedEvt ev) {
	NMEA2000Id id_60928(60928);
	std::vector<uint8_t>payload = GetN2000Payload(id_60928, ev);

	unsigned char source = payload[7];

	// Unique Identity Number 21 bits
	networkInformation[source].deviceInformation.uniqueId = (payload[index + 0] | (payload[index + 1] << 8) | (payload[index + 2] << 16) | (payload[3] << 24)) & 0x1FFFFF;

	// Manufacturer Code 11 bits
	networkInformation[source].deviceInformation.manufacturerId = ((payload[index + 0] | (payload[index + 1] << 8) | (payload[index + 2] << 16) | (payload[index + 3] << 24)) & 0xFFE00000) >> 21;

	// Not really fussed about these
	// ISO ECU Instance 3 bits()
	//networkInformation[source].deviceInformation.ecuInstance = (payload[index + 4] & 0xE0) >> 5;
	// ISO Function Instance 5 bits
	//networkInformation[source].deviceInformation.ecuFunction = payload[index + 4] & 0x1F;

	// ISO Function Class 8 bits
	networkInformation[source].deviceInformation.deviceFunction = payload[index + 5];

	// Reserved 1 bit
	//(payload[6] & 0x80) >> 7

	// Device Class 7 bits
	networkInformation[source].deviceInformation.deviceClass = payload[index + 6] & 0x7F;

	// System Instance 4 bits
	networkInformation[source].deviceInformation.deviceInstance = payload[index + 7] & 0x0F;

	// Industry Group 3 bits - Marine == 4
	networkInformation[source].deviceInformation.industryGroup = (payload[index + 7] & 0x70) >> 4;

	// ISO Self Configurable 1 bit
	networkInformation[source].deviceInformation.selfConfigure = (payload[index + 7] & 0x80) >> 7;

	// BUG BUG Debugging
	wxLogMessage(_T("Network Plugin, Address Claim: Source: %d, Id: %d, Manufacturer: %d"), source, networkInformation[source].deviceInformation.uniqueId, networkInformation[source].deviceInformation.manufacturerId);

}

// PGN 126464 Supported PGN
void NetworkPlugin::HandleN2K_126464(ObservedEvt ev) {
	NMEA2000Id id_126464(126464);
	std::vector<uint8_t>payload = GetN2000Payload(id_126464, ev);
	std::vector<unsigned int> transmittedPGN;
	std::vector<unsigned int> receivedPGN;

	unsigned char source = payload[7];
	unsigned char flagRxTx = payload[index + 0];
	// 0 = Transmitted, 1 = Received
	unsigned int pgn;
	// first byte of PGN126464 indicates whether Tx or Rx, then each PGN encoded over three bytes
	for (int i = 0; i < (((payload.size() - index - 1)) / 3); i++) {
		pgn = payload[index + 1 + (i * 3)] | (payload[index + 2 + (i * 3)] << 8) | (payload[index + 3 + (i * 3)] << 16);
		if (pgn != 0xFFFFFF) {
			if (flagRxTx == 0) {
				transmittedPGN.push_back(pgn);
			}
			if (flagRxTx == 1) {
				receivedPGN.push_back(pgn);
			}
		}
	}
	// BUG BUG What to do with these ??

}

// PGN 126993 NMEA Heartbeat
void NetworkPlugin::HandleN2K_126993(ObservedEvt ev) {
	NMEA2000Id id_126993(126993);
	std::vector<uint8_t>payload = GetN2000Payload(id_126993, ev);

	unsigned char source = payload[7];

	unsigned short timeOffset;
	timeOffset = payload[index + 0] | (payload[index + 1] << 8);

	unsigned char counter;
	counter = payload[index + 2];

	unsigned char class1CanState;
	class1CanState = payload[index + 3] & 0x07;

	unsigned char class2CanState;
	class2CanState = (payload[index + 3] & 0x38) >> 3;

	unsigned char equipmentState;
	equipmentState = (payload[index + 3] & 0x40) >> 6;

	// BUG BUG Debug
	wxLogMessage(wxString::Format("Network Plugin, Heartbeat: Source: %d, Time: %d, Count: %d, CAN 1: %d, CAN 2: %d", source, timeOffset, counter, class1CanState, class2CanState));

}

// PGN 126996 Product Information
void NetworkPlugin::HandleN2K_126996(ObservedEvt ev) {
	NMEA2000Id id_126996(126996);
	std::vector<uint8_t>payload = GetN2000Payload(id_126996, ev);

	unsigned char source = payload[7];

	// Should divide by 100 to get the correct displayable version
	networkInformation[source].productInformation.dataBaseVersion = payload[index + 0] | (payload[index + 1] << 8);

	networkInformation[source].productInformation.productCode = payload[index + 2] | (payload[index + 3] << 8);

	// Each of the following strings are up to 32 bytes long, and NOT NULL terminated.

	// Model ID Bytes [4] - [35]
	//memset(&productInformation->modelId[0], '\0', 32);
	networkInformation[source].productInformation.modelId.Clear();
	for (int j = 0; j < 31; j++) {
		if (isprint(payload[index + 4 + j])) {
			networkInformation[source].productInformation.modelId.append(1, payload[index + 4 + j]);
		}
	}

	// Software Version Bytes [36] - [67]
	//memset(&productInformation->softwareVersion[0], '\0', 32);
	networkInformation[source].productInformation.softwareVersion.Clear();
	for (int j = 0; j < 31; j++) {
		if (isprint(payload[index + 36 + j])) {
			networkInformation[source].productInformation.softwareVersion.append(1, payload[index + 36 + j]);
		}
	}

	// Model Version Bytes [68] - [99]
	//memset(&productInformation->modelVersion[0], '\0', 32);
	networkInformation[source].productInformation.modelVersion.Clear();
	for (int j = 0; j < 31; j++) {
		if (isprint(payload[index + 68 + j])) {
			networkInformation[source].productInformation.modelVersion.append(1, payload[index+ 68 + j]);
		}
	}

	// Serial Number Bytes [100] - [131]
	//memset(&productInformation->serialNumber[0], '\0', 32);
	networkInformation[source].productInformation.serialNumber.Clear();
	for (int j = 0; j < 31; j++) {
		if (isprint(payload[index + 100 + j])) {
			networkInformation[source].productInformation.serialNumber.append(1, payload[index + 100 + j]);
		}
	}

	networkInformation[source].productInformation.certificationLevel = payload[index + 132];
	networkInformation[source].productInformation.loadEquivalency = payload[133];

	// BUG BUG Debugging
	wxLogMessage(_T("Network Plugin, Product Information: Source: %d, Product Code: %d, Model Id: %s"), 
		source, networkInformation[source].productInformation.productCode, 
		networkInformation[source].productInformation.modelId);
}

// PGN 126998 Configuration Information
void NetworkPlugin::HandleN2K_126998(ObservedEvt ev) {
	NMEA2000Id id_126998(126998);
	std::vector<uint8_t>payload = GetN2000Payload(id_126998, ev);

	unsigned char source = payload[7];
	size_t variableIndex = index;

	// Use an index as each string is variable length
	unsigned int length = payload[variableIndex + 0];
	
	// BUG BUG What about Unicode strings. I have seen these once !!!

	variableIndex++;
	if (payload[variableIndex] == 1) { // First byte indicates encoding, 0 for Unicode, 1 for ASCII
		variableIndex++;
		for (size_t i = 0; i < length - 2; i++) {
			networkInformation[source].configurationInformation.information1.append(1, payload[variableIndex]);
			variableIndex++;
		}
	}

	length = payload[variableIndex];
	variableIndex++;
	if (payload[variableIndex] == 1) { // First byte indicates encoding, 0 for Unicode, 1 for ASCII
		variableIndex++;
		for (size_t i = 0; i < length - 2; i++) {
			networkInformation[source].configurationInformation.information2.append(1, payload[variableIndex]);
			variableIndex++;
		}
	}

	length = payload[variableIndex];
	variableIndex++;
	if (payload[index] == 1) { // First byte indicates encoding, 0 for Unicode, 1 for ASCII
		variableIndex++;
		for (size_t i = 0; i < length - 2; i++) {
			networkInformation[source].configurationInformation.information3.append(1, payload[variableIndex++]);
			variableIndex++;
		}
	}
	// BUG BUG Debugging
	wxLogMessage(_T("Network Plugin, Configuration Information: Source: %d, Info 1: %s, Info 2: %s"), source, 
		networkInformation[source].configurationInformation.information1, 
		networkInformation[source].configurationInformation.information2);
}
