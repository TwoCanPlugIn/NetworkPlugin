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
#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/aui/aui.h>

// Defines version numbers for this plugin
#include "version.h"

// OpenCPN include file
#include "ocpn_plugin.h"

// Displays Grid View of NMEA 2000 devices 
#include "network_pi_dialog.h"

// Toolbox Panel used for configuration settings
#include "network_pi_toolbox.h"

// Plugin receives events from the NMEA 2000 Network dialog
const wxEventType wxEVT_NETWORK_PLUGIN_EVENT = wxNewEventType();
const int NETWORKDIALOG_OPEN_EVENT = wxID_HIGHEST + 1;
const int NETWORKDIALOG_CLOSE_EVENT = wxID_HIGHEST + 2;
const int NETWORKDIALOG_PING_EVENT = wxID_HIGHEST + 3;

// Globally accessible variables

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
bool sendNetwork;

// The NMEA 2000 interface
DriverHandle driverHandle;

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
	void LateInit(void);

	void OnPaneClose(wxAuiManagerEvent& event);
	void OnPaneActivate(wxAuiManagerEvent& event);

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
	wxScrolledWindow *optionsWindow;
	NetworkToolbox *toolboxPanel;

	// Timer & Timer Events
	wxTimer *heartbeatTimer;
	void OnTimer(wxTimerEvent &event);

	// Get's the first available NMEA 2000 network interface
	wxString GetNetworkInterface(void);

	// index into the payload.
	// The payload is in Actisense format, so as I've just pasted code from twocan, 
	// the index simplifies accessing the actual data
	const int index = 13;

	// list of all PGN's we listen to
	std::vector<unsigned int> parameterGroupNumbers = { 59392, 59904, 60160,60416, 60928, 65420, 126208, 126464, 126983, 126984, 126985, 126986, 126987, 126988, 126992, 126993, 126996, 126998, 127233, 127237, 127245, 127250, 127251, 127252, 127257, 127258, 127488, 127489, 127493, 127496, 127497, 127498, 127500, 127501, 127502, 127505, 127508, 127510, 128259, 128267, 128275, 128520, 129025, 129026, 129029, 129033, 129038, 129039, 129040, 129041, 129283, 129284, 129285, 129291, 129301, 129538, 129539, 129540, 129792, 129793, 129794, 129795, 129796, 129797, 129798, 129799, 129800, 129801, 129802, 129803, 129804, 129805, 129806, 129807, 129808, 129809, 129810, 129811, 129812, 129813, 130064, 130065, 130066, 130067, 130068, 130069, 130070, 130071, 130072, 130073, 130074, 130306, 130310, 130311, 130312, 130313, 130314, 130315, 130316, 130323 };


	// Experiment, Have one handler for all received NMEA 2000 messages
	void HandleN2K(ObservedEvt ev);
	std::vector<std::shared_ptr<ObservableListener>> listeners;

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