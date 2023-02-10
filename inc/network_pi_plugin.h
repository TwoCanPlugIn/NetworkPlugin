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

// What version of the OpenCPN plugin API does this plugin support
#define     OPENCPN_API_VERSION_MAJOR    1
#define     OPENCPN_API_VERSION_MINOR    18

// OpenCPN include file
#include "ocpn_plugin.h"

// NMEA 2000 Network dialog modal window
#include "network_pi_dialog.h"
// or a non modal version
#include "network_pi_window.h"

// OCPN Toolbox Panel used for configuration settings
#include "network_pi_toolbox.h"

// Plugin receives events from the NMEA 2000 Network dialog
const wxEventType wxEVT_NETWORK_PLUGIN_EVENT = wxNewEventType();
const int NETWORKDIALOG_OPEN_EVENT = wxID_HIGHEST + 1;
const int NETWORKDIALOG_CLOSE_EVENT = wxID_HIGHEST + 2;
const int NETWORKDIALOG_PING_EVENT = wxID_HIGHEST + 3;

// NMEA 2000 Product Information, transmitted in PGN 126996 NMEA Product Information
typedef struct ProductInformation {
	unsigned int dataBaseVersion;
	unsigned int productCode;
	wxString modelId;
	wxString softwareVersion;
	wxString modelVersion;
	wxString serialNumber;
	unsigned char certificationLevel;
	unsigned char loadEquivalency;
} ProductInformation;

// NMEA 2000 Device Information, transmitted in PGN 60928 ISO Address Claim
typedef struct DeviceInformation {
	unsigned long uniqueId;
	unsigned int deviceClass;
	unsigned int deviceFunction;
	unsigned char deviceInstance;
	unsigned char industryGroup;
	unsigned int manufacturerId;
} DeviceInformation;

// Used  to store the data for the Network Map, combines elements from address claim & product information
typedef struct NetworkInformation {
	DeviceInformation deviceInformation;
	ProductInformation productInformation;
	wxDateTime timestamp; // Updated upon reception of heartbeat or address claim. Used to determine stale entries
} NetworkInformation;

// Flag used to indicate if any settings have been changed
bool settingsDirty;

// Whether the dialog is displayed
bool networkWindowIsVisible;

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
	// Network Plugin modal dialog
	NetworkDialog *networkDialog;
	// or
	// Network Plugin modeless dialog
	NetworkWindow *networkWindow;

	// Plugin bitmap
	wxBitmap pluginBitmap;

	// Reference to the OpenCPN window handle
	wxWindow *parentWindow;

	wxAuiManager *auiManager;
	wxAuiPaneInfo paneInfo;
	
	// Toolbar id's
	int networkToolbar;
	
	// Context menu id's
	int networkContextMenu;

	// Toolbox panel
	NetworkToolbox *toolboxPanel;

	// Timer & Timer Events
	wxTimer *oneMinuteTimer;
	void OnTimer();

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

	// Array of network devices & their product and device information
	// BUG BUG Array, vector or map ??
	NetworkInformation networkInformation[253];

};

#endif 

