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

// Globally accessible variables
// Plugin Configuration
wxFileConfig *configSettings;

// Configuration Settings

// Dialog visibility status, used to keep the toolbar icon state in synch
bool networkDialogIsVisible;

// Protect access to list of network devices
wxCriticalSection *lockNetworkData;

// NMEA 2000 Device Data

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
	// Maintain a reference to the OpenCPN window to use as the parent for the Race Display
	parentWindow = GetOCPNCanvasWindow();

	// Maintain a reference to the OpenCPN configuration object 
	configSettings = GetOCPNConfigObject();

	// Load toolbar icons
	wxString shareLocn = GetPluginDataDir(PLUGIN_PACKAGE_NAME) + wxFileName::GetPathSeparator() + _T("data") + wxFileName::GetPathSeparator();

	wxString normalIcon = shareLocn + _T("racing_icon_normal.svg");
	wxString toggledIcon = shareLocn + _T("racing_icon_toggled.svg");
	wxString rolloverIcon = shareLocn + _T("racing_icon_rollover.svg");

	// Initialize the toolbar id's
	networkToolbar = 0;
	
	// BUG BUG Update with new icons
	networkToolbar = InsertPlugInToolSVG(_T(""), normalIcon, rolloverIcon, toggledIcon, wxITEM_CHECK,_("Race Start Display"), _T(""), NULL, -1, 0, this);

	// Setup our NMEA 2000 Network stack
	std::vector<DriverHandle> activeDrivers = GetActiveDrivers();

	// Enumerate the drivers and select a NMEA 2000 network connection
	for (auto const& activeDriver : activeDrivers) {
		for (auto const& driver : GetAttributes(activeDriver)) {
			wxMessageBox(wxString::Format("%d: %s", driver.first, driver.second));
			if (driver.second == "NMEA2000") {
				// Save the first device as a handle 
				driverHandle = activeDriver;
				goto exitNetwork;
			}
		}

	}
exitNetwork:

	// We only transmit PGN 59904 ISO Request
	std::vector<int> pgnList{ 59904 };
	// CommDriverResult result = RegisterTXPGNs(driverHandle, pgnList);

	// Initialize NMEA 2000 NavMsg listeners

	// PGN 126996 Product Information
	wxDEFINE_EVENT(EVT_N2K_126996, ObservedEvt);
	NMEA2000Id id_126996 = NMEA2000Id(126996);
	listener_126996 = std::move(GetListener(id_126996, EVT_N2K_126996, this));
	Bind(EVT_N2K_126996, [&](ObservedEvt ev) {
		HandleN2K_126996(ev);
	});

	// PGN 126998 Configurtion Information
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
	wxMenuItem *myMenu = new wxMenuItem(NULL, wxID_HIGHEST + 1, _T("NMEA 2000"), wxEmptyString, wxITEM_NORMAL, NULL);
	networkContextMenu = AddCanvasContextMenuItem(myMenu, this);

	// Wire up the Dialog Close event
	Connect(wxEVT_NETWORK_PLUGIN_EVENT, wxCommandEventHandler(NetworkPlugin::OnPluginEvent));

	// Instantiate the dialog
	networkDialog = new  NetworkDialog(parentWindow);

	// Load our dialog into the AUI Manager
	paneInfo.Name(_T("Network Plugin"));
	paneInfo.Caption("NMEA 2000 Network");
	// BUG BUG Set the icon
	paneInfo.Float();
	paneInfo.Hide();
	paneInfo.Dockable(false);
	auiManager->AddPane(networkDialog, paneInfo);
	auiManager->Update();

	// BUG BUG Superfluous
	if (paneInfo.IsShown()) {
		networkDialogIsVisible = true;
	}
	else {
		networkDialogIsVisible = false;
	}

	// Synchronize toolbar status
	SetToolbarItemState(networkToolbar, networkDialogIsVisible);
	
	// Toolbox pane
	toolboxPanel = nullptr;

	// Protect against reading the network list being updaed
	lockNetworkData = new wxCriticalSection();
	
	// BUG BUG Investigating some different API's
	// Need to ensure dialog uses the correct units
	wxLogMessage(_T("*** Distance Unit: %s"),getUsrDistanceUnit_Plugin(-1));
	wxLogMessage(_T("*** Speed Unit: %s"), getUsrSpeedUnit_Plugin(-1));
	wxLogMessage(_T("*** Shared: %s"), *GetpSharedDataLocation());
	wxLogMessage(_T("*** Exe Path: %s"), GetOCPN_ExePath());
	wxLogMessage(_T("*** Plugin Location: %s"), *GetpPlugInLocation());
	wxLogMessage(_T("*** Private App: %s"), *GetpPrivateApplicationDataLocation());
	wxLogMessage(_T("*** Doc Path: %s"), GetWritableDocumentsDir());
	wxLogMessage(_T("*** Toolbox: %d"), GetToolboxPanelCount());
	wxLogMessage(_T("*** Data Dir: %s"), GetPluginDataDir("Race Start Display"));

	// Initialize Debug Spew via UDP
	addrLocal.Hostname();
	addrPeer.Hostname("127.0.0.1");
	addrPeer.Service(3001);

	udpSocket = new wxDatagramSocket(addrLocal, wxSOCKET_NONE);
	if (!udpSocket->IsOk()) {
		wxLogMessage(_T("ERROR: failed to create UDP peer socket"));
	}

	// Start a timer to transmit NMEA 2000 network queries
	oneMinuteTimer = new wxTimer();
	oneMinuteTimer->Connect(oneMinuteTimer->GetId(), wxEVT_TIMER, wxTimerEventHandler(NetworkWindow::OnTimer), NULL, this);
	oneMinuteTimer->Start(60000);

	// Notify OpenCPN of our capabilities and what events we want to receive callbacks for
	return (WANTS_CONFIG | WANTS_TOOLBAR_CALLBACK | INSTALLS_TOOLBAR_TOOL | INSTALLS_CONTEXTMENU_ITEMS | WANTS_CURSOR_LATLON | WANTS_OVERLAY_CALLBACK | INSTALLS_TOOLBOX_PAGE | WANTS_NMEA_EVENTS | WANTS_NMEA_SENTENCES | WANTS_AIS_SENTENCES | WANTS_LATE_INIT | WANTS_PLUGIN_MESSAGING);
}

void NetworkPlugin::OnTimer() {
	std::vector<uint8_t> payload;

	payload.push_back(60928 & 0xFF);
	payload.push_back((60928 >> 8) & 0xFF);
	payload.push_back((60928 >> 16) & 0xFF);

	auto sharedPointer = std::make_shared<std::vector<uint8_t> >(std::move(payload));

	WriteCommDriverN2K(driverHandle, 59904, 255, 5, sharedPointer);

	payload.clear();
	payload.push_back(126996 & 0xFF);
	payload.push_back((126996 >> 8) & 0xFF);
	payload.push_back((126996 >> 16) & 0xFF);

	sharedPointer = std::make_shared<std::vector<uint8_t> >(std::move(payload));

	WriteCommDriverN2K(driverHandle, 59904, 255, 5, sharedPointer);

	payload.clear();
	payload.push_back(126998 & 0xFF);
	payload.push_back((126998 >> 8) & 0xFF);
	payload.push_back((126998 >> 16) & 0xFF);

	sharedPointer = std::make_shared<std::vector<uint8_t> >(std::move(payload));

	WriteCommDriverN2K(driverHandle, 59904, 255, 5, sharedPointer);

}

void NetworkPlugin::OnPaneClose(wxAuiManagerEvent& event) {
	networkDialogIsVisible = false;
	SetToolbarItemState(networkToolbar, networkDialogIsVisible);
}

// OpenCPN is either closing down, or has been disabled from the Preferences Dialog
bool NetworkPlugin::DeInit(void) {
	// Cleanup Dialog Event Handler
	Disconnect(wxEVT_NETWORK_PLUGIN_EVENT, wxCommandEventHandler(NetworkPlugin::OnPluginEvent));

	// Stop the timer
	oneMinuteTimer->Stop();
	oneMinuteTimer->Disconnect(wxEVT_TIMER, wxTimerEventHandler(NetworkWindow::OnTimer), NULL, this);
	delete oneMinuteTimer;

	// Cleanup the critical section lock
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
	if ((paneInfo.IsOk()) && (paneInfo.IsShown())) {
		networkDialogIsVisible = true;
		SetToolbarItemState(networkToolbar, networkDialogIsVisible);
	}
}

void NetworkPlugin::LateInit(void) {
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
	return _T("Network Plugin, Displays NMEA 2000 Network");
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
	wxMessageBox(wxString::Format(_T("OnCloseToolbox: %d"), ok_apply_cancel));
	if ((ok_apply_cancel == 0) || (ok_apply_cancel == 4) && (settingsDirty == TRUE)) {
			// Save the setttings
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
void NetworkPlugin::OnContextMenuItemCallback(int id) {
	if (id == networkContextMenu) {	
		// BUG BUG Meaningless
		wxMessageBox(wxString::Format(_T("Chart Tilt: %f"), GetCanvasTilt()));
	}
}

// Invoked by OpenCPN whenever a toolbar item is selected
// Requires WANTS_TOOLBAR_CALLBACK
void NetworkPlugin::OnToolbarToolCallback(int id) {
	// Display modal Race Start Window
	//RacingDialog *racingDialog = new RacingDialog(parentWindow);
	//racingDialog->ShowModal();
	//delete racingDialog;
	//SetToolbarItemState(id, false);

	if (id == networkToolbar) {
		// Display a non-modal Race Start Window
		if (!networkDialogIsVisible) {
			networkWindow = new NetworkWindow(parentWindow, this);
			networkDialogIsVisible = true;
			SetToolbarItemState(id, networkDialogIsVisible);
			networkWindow->Show(true);
		}
		else {
			networkWindow->Close();
			delete networkWindow;
			SetToolbarItemState(id, networkDialogIsVisible);
		}
	}
}

// Handle events from the dialog
void NetworkPlugin::OnPluginEvent(wxCommandEvent &event) {
	switch (event.GetId()) {
		case NETWORKDIALOG_OPEN_EVENT:
			// Synchronises the state of the toolbar icon
			SetToolbarItemState(networkToolbar, networkDialogIsVisible);
			break;
		case NETWORKDIALOG_CLOSE_EVENT:
			// Synchronises the state of the toolbar icon
			SetToolbarItemState(networkToolbar, networkDialogIsVisible);
			break;
		case NETWORKDIALOG_PING_EVENT:
			lockNetworkData->Leave();
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

// PGN 126464 Supported PGN
void NetworkPlugin::HandleN2K_126464(ObservedEvt ev) {
	NMEA2000Id id_126464(126464);
	std::vector<uint8_t>payload = GetN2000Payload(id_126464, ev);
}

// PGN 126993 NMEA Heartbeat
void NetworkPlugin::HandleN2K_126993(ObservedEvt ev) {
	NMEA2000Id id_126993(126993);
	std::vector<uint8_t>payload = GetN2000Payload(id_126993, ev);
}

// PGN 126996 Product Information
void NetworkPlugin::HandleN2K_126996(ObservedEvt ev) {
	NMEA2000Id id_126996(126996);
	std::vector<uint8_t>payload = GetN2000Payload(id_126996, ev);

}

// PGN 126998 Configuration Information
void NetworkPlugin::HandleN2K_126998(ObservedEvt ev) {
	NMEA2000Id id_126998(126998);
	std::vector<uint8_t>payload = GetN2000Payload(id_126998, ev);
}
