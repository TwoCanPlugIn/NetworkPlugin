// Copyright(C) 2018-2020 by Steven Adler
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


#ifndef NETWORK_PLUGIN_H
#define NETWORK_PLUGIN_H

// Pre compiled headers 
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
      #include <wx/wx.h>
#endif

// Necessary wxWidgets headers
#include <wx/graphics.h>

#include <wx/math.h>

#include <wx/aui/aui.h>

#include <wx/socket.h>

#include <wx/config.h>
#include <wx/fileconf.h>

// Defines version numbers for this plugin
#include "version.h"

// OpenCPN include file
#include "ocpn_plugin.h"

// NMEA 2000 Network 
#include "network_pi_dialog.h"

// Open CPN Toolbox Panel used for configuration settings
#include "network_pi_toolbox.h"

// Plugin receives events from the NMEA 2000 Network dialog
const wxEventType wxEVT_NETWORK_PLUGIN_EVENT = wxNewEventType();
const int NETWORKDIALOG_OPEN_EVENT = wxID_HIGHEST + 1;
const int NETWORKDIALOG_CLOSE_EVENT = wxID_HIGHEST + 2;
const int NETWORKDIALOG_PING_EVENT = wxID_HIGHEST + 3;

// Globally accessible variables

// Protect access to list of network devices
// BUG BUG Is this used anywhere ??
wxCriticalSection *lockNetworkData;

// Array of network devices & their product and device information
NetworkInformation networkInformation[253];

// Plugin Configuration
wxFileConfig *configSettings;

// Dialog visibility status, used to keep the toolbar icon state in synch
bool isNetworkDialogVisible;

// Flag used to indicate if any settings have been changed
bool settingsDirty;

// Frequency for sending heartbeat & network requests
int heartbeatInterval;

// Whether to send PGN 126233 Hearbeats
bool sendHeartbeat;

// Whether to request PGN 126996, 126998
bool sendRequest;

// The NMEA 2000 interface
wxString interfaceName;

// The Network plugin
class NetworkPlugin : public opencpn_plugin_118, public wxEvtHandler {

public:
	// The constructor
	NetworkPlugin(void *ppimgr);
	
	// and destructor
	~NetworkPlugin(void);

	// OpenCPN plugin methods
	int Init(void);
	bool DeInit(void);
	int GetAPIVersionMajor();
	int GetAPIVersionMinor();
	int GetPlugInVersionMajor();
	int GetPlugInVersionMinor();

	wxString GetCommonName();
	wxString GetShortDescription();
	wxString GetLongDescription();
	wxBitmap *GetPlugInBitmap();
	
	int GetToolbarToolCount(void);
	void OnToolbarToolCallback(int id);
	void OnContextMenuItemCallback(int id);
	void SetDefaults(void);	
	void OnSetupOptions(void);
	void SetupToolboxPanel(int page_sel, wxNotebook* pnotebook);
	void OnCloseToolboxPanel(int page_sel, int ok_apply_cancel);
	void UpdateAuiStatus(void);
	void OnPaneClose(wxAuiManagerEvent& event);
	void LateInit(void);

	// Event Handler
	void OnPluginEvent(wxCommandEvent &event);
		
private:
	// AUI Panel with a Data Grid to display list of NNMEA 2000 Network Devices
	NetworkDialog *networkDialog;

	// Plugin bitmap
	wxBitmap pluginBitmap;

	// Reference to the OpenCPN window handle
	wxWindow *parentWindow;

	// wxWidgets Advanced User Interface stuff
	wxAuiManager *auiManager;
	wxAuiPaneInfo paneInfo;
	
	// Toolbar id's
	int networkToolbar;
	
	// Context menu id's
	int networkContextMenu;

	// Toolbox panel, displayed in OpenCPN Settings dialog
	NetworkToolbox *toolboxPanel;

	// Timer & Timer Events
	wxTimer *heartbeatTimer;
	void OnTimer(wxTimerEvent &event);

	// NMEA 2000
	DriverHandle driverHandle;

	// index into the payload.
	// The payload is in Actisense format, so as I've just pasted code from twocan, 
	// the index simplifies accessing the actual data
	const int index = 13;

	// ISO Address Claim
	void HandleN2K_60928(ObservedEvt ev);
	std::shared_ptr<ObservableListener> listener_60928;

	// NMEA Supported PGN
	void HandleN2K_126464(ObservedEvt ev);
	std::shared_ptr<ObservableListener> listener_126464;

	// NMEA Heartbeat
	void HandleN2K_126993(ObservedEvt ev);
	std::shared_ptr<ObservableListener> listener_126993;

	// NMEA Product Information
	void HandleN2K_126996(ObservedEvt ev);
	std::shared_ptr<ObservableListener> listener_126996;

	// NMEA Configuration Information
	void HandleN2K_126998(ObservedEvt ev);
	std::shared_ptr<ObservableListener> listener_126998;

};

#endif 

