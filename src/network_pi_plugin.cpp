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

}

// Destructor
NetworkPlugin::~NetworkPlugin(void) {
	// Nothing to do ??
}

int NetworkPlugin::Init(void) {
	// Maintain a reference to the OpenCPN window to use as the parent for the dialog
	parentWindow = GetOCPNCanvasWindow();

	// Maintain a reference to the OpenCPN configuration object 
	configSettings = GetOCPNConfigObject();

	// Maintain a reference to the Advanced User Interface Manager (AUI)
	auiManager = GetFrameAuiManager();

	// Load the configuration settings
	if (configSettings) {
		configSettings->SetPath(_T("/PlugIns/Network"));
		configSettings->Read(_T("Visible"), &isNetworkDialogVisible, FALSE);
		configSettings->Read(_T("Interval"), &heartbeatInterval, 0);
		//wxString tempString;
		//configSettings->Read(_T("Interface"), &tempString, wxEmptyString);
		//driverHandle = std::string(tempString.mb_str());
		driverN2K = GetNetworkInterface();
		driverSignalK = GetSignalKInterface();
		configSettings->Read(_T("Heartbeat"), &sendHeartbeat, FALSE);
		configSettings->Read(_T("Request"), &sendNetwork, FALSE);
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
	result = RegisterTXPGNs(driverN2K, pgnList);

	// Initialize NMEA 2000 Listeners

	// Experimental, listen for all NMEA 2000 Messages
	wxDEFINE_EVENT(EVT_N2K, ObservedEvt);
	for (auto& it : parameterGroupNumbers) {
		wxLogMessage(_T("Added Listener: %d, %s"), it.first, it.second);
		listeners.push_back(std::move(GetListener(NMEA2000Id(it.first), EVT_N2K, this)));
	}

	Bind(EVT_N2K, [&](ObservedEvt ev) {
		HandleN2K(ev);
	});

	/*

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

	*/

	// Example of adding context menu items including separators etc.
	// BUG BUG Remove ??
	wxMenuItem *myMenu = new wxMenuItem(NULL, wxID_HIGHEST + 1, _T("NMEA 2000"), wxEmptyString, wxITEM_NORMAL, NULL);
	networkContextMenu = AddCanvasContextMenuItem(myMenu, this);

	// Wire up the Network Dialog Close event
	Connect(wxEVT_NETWORK_PLUGIN_EVENT, wxCommandEventHandler(NetworkPlugin::OnPluginEvent));

	// Instantiate the dialog
	networkDialog = new  NetworkDialog(parentWindow, this);

	
	// BUG BUG Superfluous
	if ((paneInfo.IsOk()) && (paneInfo.IsShown())) {
		isNetworkDialogVisible = TRUE;
	}
	else {
		isNetworkDialogVisible = FALSE;
	}

	// Synchronize toolbar status
	SetToolbarItemState(networkToolbar, isNetworkDialogVisible);
	
	// OpenCPN Settings Dialog toolbox extension
	toolboxPanel = nullptr;

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

	// Start a timer to transmit NMEA 2000 network queries
	if ((sendHeartbeat) && ((!driverN2K.empty()) || (!driverSignalK.empty()) ) ){
		heartbeatTimer = new wxTimer();
		heartbeatTimer->Connect(heartbeatTimer->GetId(), wxEVT_TIMER, wxTimerEventHandler(NetworkPlugin::OnTimer), NULL, this);
		heartbeatTimer->Start(heartbeatInterval * 60000);
	}

	// Notify OpenCPN of our capabilities and what events we want to receive callbacks for
	return (WANTS_CONFIG | WANTS_TOOLBAR_CALLBACK | INSTALLS_TOOLBAR_TOOL | INSTALLS_CONTEXTMENU_ITEMS | USES_AUI_MANAGER | INSTALLS_TOOLBOX_PAGE | WANTS_LATE_INIT);
}

void NetworkPlugin::LateInit(void) {
	// Load our dialog into the AUI Manager
	paneInfo.Name(_T("Network Plugin"));
	paneInfo.Caption("NMEA 2000 Network");
	paneInfo.Float();
	paneInfo.Hide();
	paneInfo.Dockable(FALSE);
	auiManager->AddPane(networkDialog, paneInfo);
	auiManager->Connect(wxEVT_AUI_PANE_CLOSE, wxAuiManagerEventHandler(NetworkPlugin::OnPaneClose), NULL, this);
	auiManager->Connect(wxEVT_AUI_PANE_ACTIVATED, wxAuiManagerEventHandler(NetworkPlugin::OnPaneActivate), NULL, this);
	auiManager->Update();

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
		wxString tmpString(driverN2K.c_str(), wxConvUTF8);
		configSettings->Write(_T("Interface"), tmpString);
		configSettings->Write(_T("Heartbeat"), sendHeartbeat);
		configSettings->Write(_T("Request"), sendNetwork);
	}

	// Stop the timer
	if (sendHeartbeat) {
		heartbeatTimer->Stop();
		heartbeatTimer->Disconnect(wxEVT_TIMER, wxTimerEventHandler(NetworkPlugin::OnTimer), NULL, this);
		delete heartbeatTimer;
	}

	// Cleanup AUI
	auiManager->Disconnect(wxEVT_AUI_PANE_CLOSE, wxAuiManagerEventHandler(NetworkPlugin::OnPaneClose), NULL, this);
	auiManager->Disconnect(wxEVT_AUI_PANE_ACTIVATED, wxAuiManagerEventHandler(NetworkPlugin::OnPaneActivate), NULL, this);
	auiManager->UnInit();
	auiManager->DetachPane(networkDialog);
	delete networkDialog;

	return TRUE;
}

// Alternative
void NetworkPlugin::SendSignalkLogon(void) {
	
	std::string data = "{\"requestId\":\"FA1CA3B7-F121-4E5C-99FA-A498BD5CAFEB\",\"login\":{\"username\":\"pi\",\"password\":\"raspberry\"}}";
	auto payload = make_shared<std::vector<uint8_t>>();
	for (const auto& ch : data) {
		payload->push_back(ch);
	}
	CommDriverResult outcome = WriteCommDriver(driverSignalK, payload);

	wxLogMessage(_T("Send Signalk Logon: %d"), outcome);


}

// Send a SignalK Logon
/*
void NetworkPlugin::SendSignalkLogon(void) {
	CommDriverResult result;
	
	wxString message = "{\"requestId\":\"FA1CA3B7-F121-4E5C-99FA-A498BD5CAFEB\",\"login\":{\"username\":\"pi\",\"password\":\"raspberry\"}}";
	
	wxLogMessage(_T("SignalK Logon: %s"), message);
	
	std::vector<unsigned char>SignalK;
	for (auto it : message) {
		SignalK.push_back(it);
	}
	auto signalkPointer = std::make_shared<std::vector<uint8_t> >(std::move(SignalK));
	result = WriteCommDriver(driverSignalK, signalkPointer);

	wxLogMessage(_T("### Send SignalK: %s, %d"), driverSignalK.c_str(), result);

}

*/

// Send a SignalK Update
void NetworkPlugin::SendSignalkUpdate(void) {
	CommDriverResult result;

	wxString message = "{\"updates\":[{\"source\":{\"label\":\"CAN-BUS\",\"type\":\"NMEA2000\",\"pgn\":127250,\"src\":\"2\",\"deviceInstance\":0},\"values\":[{\"path\":\"navigation.headingTrue\",\"value\":2.0}]}],\"context\":\"vessels.self\"}";

	wxLogMessage(_T("SignalK Update: %s"), message);

	std::vector<unsigned char>SignalK;
	for (auto it : message) {
		SignalK.push_back(it);
	}
	auto signalkPointer = std::make_shared<std::vector<uint8_t> >(std::move(SignalK));
	result = WriteCommDriver(driverSignalK, signalkPointer);

	wxLogMessage(_T("### Send SignalK: %s, %d"), driverSignalK.c_str(), result);

}

void NetworkPlugin::SendNMEA2000(void) {
	CommDriverResult result;

	std::vector<uint8_t> payload;

	payload.push_back(60928 & 0xFF);
	payload.push_back((60928 >> 8) & 0xFF);
	payload.push_back((60928 >> 16) & 0xFF);

	auto sharedPointer = std::make_shared<std::vector<uint8_t> >(std::move(payload));

	//result = WriteCommDriverN2K(driverHandle, 59904, 255, 5, sharedPointer);
	//wxLogMessage(_T("Network Plugin, Write ISO Request for 60928: %d"), result);

	payload.clear();
	payload.push_back(126996 & 0xFF);
	payload.push_back((126996 >> 8) & 0xFF);
	payload.push_back((126996 >> 16) & 0xFF);

	sharedPointer = std::make_shared<std::vector<uint8_t> >(std::move(payload));

	//result = WriteCommDriverN2K(driverHandle, 59904, 255, 5, sharedPointer);
	//wxLogMessage(_T("Network Plugin, Write ISO Request for 126996: %d"), result);

	payload.clear();
	payload.push_back(126998 & 0xFF);
	payload.push_back((126998 >> 8) & 0xFF);
	payload.push_back((126998 >> 16) & 0xFF);

	sharedPointer = std::make_shared<std::vector<uint8_t> >(std::move(payload));

	//result = WriteCommDriverN2K(driverHandle, 59904, 255, 5, sharedPointer);
	//wxLogMessage(_T("Network Plugin, Write ISO Request for 126998: %d"), result);

	payload.clear();

	int latitude = 38.0f * 1e7;
	int longitude = 145.0f * 1e7;

	payload.push_back(latitude & 0xFF);
	payload.push_back((latitude >> 8) & 0xFF);
	payload.push_back((latitude >> 16) & 0xFF);
	payload.push_back((latitude >> 24) & 0xFF);

	payload.push_back(longitude & 0xFF);
	payload.push_back((longitude >> 8) & 0xFF);
	payload.push_back((longitude >> 16) & 0xFF);
	payload.push_back((longitude >> 24) & 0xFF);

	sharedPointer = std::make_shared<std::vector<uint8_t> >(std::move(payload));

	//result = WriteCommDriverN2K(driverN2K, 129025, 255, 5, sharedPointer);
	//wxLogMessage(_T("Network Plugin, Write PGN 129025: %d"), result);

	payload.clear();

	unsigned short dbver = 2100;
	payload.push_back(dbver & 0xFF);
	payload.push_back((dbver >> 8) & 0xFF);

	unsigned short pcode = 1234;
	payload.push_back(pcode & 0xFF);
	payload.push_back((pcode >> 8) & 0xFF);

	// Model Id
	payload.push_back(0x41);
	payload.push_back(0x41);
	payload.push_back(0x41);
	payload.push_back(0x41);

	for (size_t i = 0; i < 28; i++) {
		payload.push_back(0x20);
	}

	// Software Version Bytes [36] - [67]
	payload.push_back(0x42);
	payload.push_back(0x42);
	payload.push_back(0x42);
	payload.push_back(0x42);

	for (size_t i = 0; i < 28; i++) {
		payload.push_back(0x20);
	}

	// Model Version Bytes [68] - [99]
	payload.push_back(0x43);
	payload.push_back(0x43);
	payload.push_back(0x43);
	payload.push_back(0x43);

	for (size_t i = 0; i < 28; i++) {
		payload.push_back(0x20);
	}

	// Serial Number Bytes [100] - [131] - Let's reuse our uniqueId as the serial number
	payload.push_back(0x44);
	payload.push_back(0x44);
	payload.push_back(0x44);
	payload.push_back(0x44);

	for (size_t i = 0; i < 28; i++) {
		payload.push_back(0x20);
	}
	payload.push_back(0x01); //CONST_CERTIFICATION_LEVEL;

	payload.push_back(0x01); // CONST_LOAD_EQUIVALENCY;

	sharedPointer = std::make_shared<std::vector<uint8_t> >(std::move(payload));

	//result = WriteCommDriverN2K(driverHandle, 126996, 255, 5, sharedPointer);
	//wxLogMessage(_T("Network Plugin, Transmit PGN 126996: %d"), result);
}



// Timer sends heartbeat and network request PGN's
void NetworkPlugin::OnTimer(wxTimerEvent &event) {

	wxLogMessage(_T("Network Plugin - On Timer"));
	wxLog::FlushActive();
	
	SendSignalkLogon();

	wxLogMessage(_T("Network Plugin, After SignalK Logon"));
	wxLog::FlushActive();


	SendSignalkUpdate();

	wxLogMessage(_T("Network Plugin, After SignalK Update"));
	wxLog::FlushActive();

	SendNMEA2000();
	
	wxLogMessage(_T("Network Plugin, After NMEA2000"));
	wxLog::FlushActive();
}

// Called when OpenCPN is loading saved AUI pages
void NetworkPlugin::UpdateAuiStatus(void) {
	if ((paneInfo.IsOk()) && (paneInfo.IsShown())) {
		isNetworkDialogVisible = TRUE;
		SetToolbarItemState(networkToolbar, isNetworkDialogVisible);
	}
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
	return _T(PLUGIN_COMMON_NAME);
}

wxString NetworkPlugin::GetShortDescription() {
	return _T(PLUGIN_SHORT_DESCRIPTION);
}

wxString NetworkPlugin::GetLongDescription() {
	return _T(PLUGIN_LONG_DESCRIPTION);
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
	optionsWindow = AddOptionsPage(OptionsParentPI::PI_OPTIONS_PARENT_UI, _T("Network"));
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	optionsWindow->SetSizer(sizer);
	// Create our toolbox panel and add it to the toolbox via the sizer
	toolboxPanel = new NetworkToolbox(optionsWindow);
	sizer->Add(toolboxPanel, 1, wxALL | wxEXPAND);
	// Set toolbox UI values
	toolboxPanel->SetInterval(heartbeatInterval);
	toolboxPanel->SetHeartbeat(sendHeartbeat);
	toolboxPanel->SetNetwork(sendNetwork);
	toolboxPanel->SetInterface(driverN2K);
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
			
			if (settingsDirty == TRUE) {

				heartbeatInterval = toolboxPanel->GetInterval();
				sendHeartbeat = toolboxPanel->GetHeartbeat();
				sendNetwork = toolboxPanel->GetNetwork();
				driverN2K = toolboxPanel->GetInterface();

				configSettings->Write(_T("Interval"), heartbeatInterval);
				wxString tmpString(driverN2K.c_str(), wxConvUTF8);
				configSettings->Write(_T("Interface"), tmpString);
				configSettings->Write(_T("Heartbeat"), sendHeartbeat);
				configSettings->Write(_T("Request"), sendNetwork);
				if (heartbeatTimer != nullptr) {
					heartbeatTimer->Start(heartbeatInterval * 60000);
				}
			}
		}
	}

	// Cleanup the toolbox
	delete toolboxPanel;
	toolboxPanel = nullptr;
	DeleteOptionsPage(optionsWindow);
}

// Return the number of toolbar icons the plugin installs
int NetworkPlugin::GetToolbarToolCount(void) {
 return 1;
}

// Invoked whenever the plugin is enabled from the toolbox
// Requires WANTS_CONFIG
void NetworkPlugin::SetDefaults(void) {
	// Is called when the plugin is enabled
	wxMessageBox("Set Defaults called");
}

// Invoked whenever a context menu item is selected
// Requires INSTALLS_CONTEXTMENU_ITEMS
// BUG BUG Remove as not useful
void NetworkPlugin::OnContextMenuItemCallback(int id) {
	if (id == networkContextMenu) {	
		//isNetworkDialogVisible = !isNetworkDialogVisible;
		//auiManager->GetPane(_T("Network Plugin")).Show(isNetworkDialogVisible);
		//auiManager->Update();
		//SetToolbarItemState(id, isNetworkDialogVisible);
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
		auiManager->GetPane(_T("Network Plugin")).Centre();
		auiManager->Update();
		SetToolbarItemState(id, isNetworkDialogVisible);
	}
}

// Keep the toolbar in synch with the AUI Manager
void NetworkPlugin::OnPaneClose(wxAuiManagerEvent& event) {
	isNetworkDialogVisible = FALSE;
	SetToolbarItemState(networkToolbar, isNetworkDialogVisible);
}

// Intent is to force the loading of the network map into the dialog
void NetworkPlugin::OnPaneActivate(wxAuiManagerEvent& event) {
	wxMessageBox("AUI Activate");
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
			wxLogMessage(_T("**** DEBUG DIALOG_CLOSE EVENT ***"));
			isNetworkDialogVisible = FALSE;
			SetToolbarItemState(networkToolbar, isNetworkDialogVisible);
			break;
		case NETWORKDIALOG_PING_EVENT:
			// Intent is to "Ping" the NMEA 2000 device
			break;
		default:
			event.Skip();
	}
}

wxString NetworkPlugin::GetNetworkInterface(void) {
	// retrieves the first NMEA 2000 Network Interface
	std::vector<DriverHandle> activeDrivers;
	activeDrivers = GetActiveDrivers();
	// Enumerate the drivers and select the first NMEA 2000 network connection
	for (auto const& activeDriver : activeDrivers) {
		for (auto const& driver : GetAttributes(activeDriver)) {
			if (driver.second == "nmea2000") {
				return activeDriver;
			}
		}
	}
	return wxEmptyString;
}

wxString NetworkPlugin::GetSignalKInterface(void) {
	// retrieves the first SignalK Network Interface
	std::vector<DriverHandle> activeDrivers;
	activeDrivers = GetActiveDrivers();
	// Enumerate the drivers and select the first NMEA 2000 network connection
	for (auto const& activeDriver : activeDrivers) {
		wxLogMessage(_T("Enumerating %s"), activeDriver.c_str());
		for (auto const& driver : GetAttributes(activeDriver)) {
			if (driver.second == "SignalK") {
				wxLogMessage(_T("Found and Using %s"), activeDriver);
				return activeDriver;
			}
		}
	}
	return wxEmptyString;
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

void NetworkPlugin::HandleN2K(ObservedEvt ev) {
	std::vector<uint8_t>payload = GetN2000Payload(NMEA2000Id(123456), ev);
	ParseMessage(payload);
}	

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

// We should dislay this to a debug/diagnostics window
wxString NetworkPlugin::ParseMessage(std::vector<uint8_t> data) {
	unsigned int pgn = data[3] | (data[4] << 8) | (data[5] << 16);
	wxString result = wxString::Format("PGN: %d (%s), Source: %d, Destination: %d, Priority: %d", pgn, parameterGroupNumbers[pgn], data[2], data[6], data[7]);
	wxLogMessage(result);
	return result;
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
