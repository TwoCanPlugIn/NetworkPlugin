// Copyright(C) 2023-2024 by Steven Adler
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

// STL 
#include <map>

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

// Day, Dusk & Night mode constants
const int NAVICO_DAY_MODE = 2;
const int NAVICO_DUSK_MODE = 4;
const int NAVICO_NIGHT_MODE = 4;

const int GARMIN_DAY_MODE = 0;
const int GARMIN_DUSK_MODE = 1;
const int GARMIN_NIGHT_MODE = 1;

// Globally accessible variables

// Array of network devices & their product and device information
std::vector<NetworkDevice> networkDevices;

// Plugin Configuration
wxFileConfig *configSettings;

// Dialog visibility status, used to keep the toolbar icon state in synch
bool isNetworkDialogVisible;

// Frequency for sending heartbeat & network requests
int heartbeatInterval;

// Whether to send PGN 126233 Hearbeats
bool sendHeartbeat;

// Whether to request PGN 126996, 126998
bool sendNetwork;

// Set the day/night mode for external instruments
bool synchronizeDisplays;

// Flags for each of the different vendors
bool displayNavico;
bool displayRaymarine;
bool displayGarmin;

// The NMEA 2000 interfaces
DriverHandle driverHandleN2K;
DriverHandle driverHandleSignalK;

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
	void SetColorScheme(PI_ColorScheme cs);
	bool QueryShutDown(void);
	
	// AUI Event Handlers
	void OnPaneClose(wxAuiManagerEvent& event);

	// Network Plugin event handler
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

	// Get the first available NMEA 2000 network interface
	wxString GetNetworkInterface(void);

	// Transmit ISO Requests (59904) for Address Claim (60928) 
	// Product Information (126996) & Heartbeat (126993)
	void SendNMEA2000(void);

	// Get's the first available SignalK Interface
	wxString GetSignalKInterface(void);

	// Send SignalK Logon and Update Messages
	void SendSignalkLogon(void);
	void SendSignalkUnsubscribe(bool subscribe);
	void SendSignalkUpdate(void);

	// Change Brightness & Colour Mode for other instrument displays
	bool SetNavicoDisplayBrightness(int brightness, int group);
	bool SetNavicoDisplayMode(int mode, int group);
	bool SetNavicoDisplayColour(int colour, int group);
	bool SetGarminDisplayMode(int mode);
	bool SetGarminDisplayColour(int colour);
	bool SetGarminDisplayBrightness(int brightness, int mode);
	bool SetRaymarineDisplayMode(int mode, int group);
	bool SetRaymarineDisplayBrightness(int brightness, int group);

	// index into the payload.
	// The payload is in Actisense format, so as I've just pasted code from twocan, 
	// the index simplifies accessing the actual data
	const int index = 13;

	// list of all PGN's we listen to
	std::map<unsigned int, wxString> parameterGroupNumbers = {
		{59392, "ISO Acknowledgement"},
		{59904, "ISO Request"},
		{60160, "ISO Transport Protocol, Data Transfer"},
		{60416, "ISO Transport Protocol, Connection Management"},
		{60928, "ISO Address Claim"},
		{61184, "Manufacturer Proprietary Single Frame"},
		{65280, "Manufacturer Proprietary Single Frame"},
		{65305, "Manufacturer Proprietary Single Frame (B&G AC12 Autopilot Status)"},
		{65309, "Manufacturer Proprietary Single Frame (B&G WS320 Battery Status)"},
		{65312, "Manufacturer Proprietary Single Frame (B&G WS320 Wireless Status)"},
		{65340, "Manufacturer Proprietary Single Frame (B&G AC12 Autopilot Mode)"},
		{65341, "Manufacturer Proprietary Single Frame (B&G AC12 Wind Angle)"},
		{65359, "Manufacturer Proprietary Single Frame (Seatalk Pilot Heading)"},
		{65360, "Manufacturer Proprietary Single Frame (Seatalk Target Heading)"},
		{65379, "Manufacturer Proprietary Single Frame (Seatalk Pilot Mode)"},
		{65380, "Manufacturer Proprietary Single Frame"},
		{65384, "Manufacturer Proprietary Single Frame"},
		{65420, "ISO Commanded Address"},
		{126208, "NMEA Group Function"},
		{126464, "PGN List"},
		{126720, "Manufacturer Proprietary Fast Packet"},
		{126983, "Alert"},
		{126984, "Alert Response"},
		{126985, "Alert Text"},
		{126986, "Alert Configuration"},
		{126987, "Alert Threshold"},
		{126988, "Alert Value"},
		{126992, "System Time"},
		{126993, "Heartbeat"},
		{126996, "Product Information"},
		{126998, "Configuration Information"},
		{127233, "Man Overboard Notification (MOB)"},
		{127237, "Heading/Track Control"},
		{127245, "Rudder"},
		{127250, "Vessel Heading"},
		{127251, "Rate of Turn"},
		{127252, "Heave"},
		{127257, "Attitude"},
		{127258, "Magnetic Variation"},
		{127488, "Engine Parameters, Rapid Update"},
		{127489, "Engine Parameters, Dynamic"},
		{127493, "Transmission Parameters, Dynamic"},
		{127496, "Trip Fuel Consumption, Vessel"},
		{127497, "Trip Fuel Consumption, Engine"},
		{127498, "Engine Parameters, Static"},
		{127500, "Load Controller Connection State / Control"},
		{127501, "Binary Status Report"},
		{127502, "Switch Bank Control"},
		{127503, "AC Input Status - DEPRECATED"},
		{127504, "AC Output Status - DEPRECATED"},
		{127505, "Fluid Level"},
		{127506, "DC Detailed Status"},
		{127507, "Charger Status - DEPRECATED"},
		{127508, "Battery Status"},
		{127509, "Inverter Status - DEPRECATED"},
		{127510, "Charger Configuration Status"},
		{127511, "Inverter Configuration Status"},
		{127512, "AGS Configuration Status"},
		{127513, "Battery Configuration Status"},
		{127514, "AGS Status"},
		{127744, "AC Power / Current - Phase A"},
		{127745, "AC Power / Current - Phase B"},
		{127746, "AC Power / Current - Phase C"},
		{127747, "AC Voltage / Frequency - Phase A"},
		{127748, "AC Voltage / Frequency - Phase B"},
		{127749, "AC Voltage / Frequency - Phase C"},
		{127750, "Converter (Inverter/Charger) Status"},
		{127751, "DC Voltage / Current"},
		{128259, "Speed, Water Referenced"},
		{128267, "Water Depth"},
		{128275, "Distance Log"},
		{128520, "Tracked Target Data"},
		{129025, "Position, Rapid Update"},
		{129026, "COG & SOG, Rapid Update"},
		{129027, "Position Delta, High Precision Rapid Update"},
		{129028, "Altitude Delta, High Precision Rapid Update"},
		{129029, "GNSS Position Data"},
		{129033, "Local Time Offset"},
		{129038, "AIS Class A Position Report"},
		{129039, "AIS Class B Position Report"},
		{129040, "AIS Class B Extended Position Report"},
		{129041, "AIS Aids To Navigation (AtoN) Report"},
		{129044, "Datum"},
		{129045, "User Datum Settings"},
		{129283, "Cross Track Error"},
		{129284, "Navigation Data"},
		{129285, "Navigation - Route / WP information"},
		{129291, "Set & Drift, Rapid Update"},
		{129301, "Time to / from Mark"},
		{129302, "Bearing and Distance between two Marks"},
		{129538, "GNSS Control Status"},
		{129539, "GNSS DOPs"},
		{129540, "GNSS Satellites in View"},
		{129541, "GPS Almanac Data"},
		{129542, "GNSS Pseudorange Noise Statistics"},
		{129545, "GNSS RAIM Output"},
		{129546, "GNSS RAIM Settings"},
		{129547, "GNSS Pseudorange Error Statistics"},
		{129549, "DGNSS Corrections"},
		{129550, "GNSS Differential Correction Receiver Interface"},
		{129551, "GNSS Differential Correction Receiver Signal"},
		{129556, "GLONASS Almanac Data"},
		{129792, "AIS DGNSS Broadcast Binary Message"},
		{129793, "AIS UTC and Date Report"},
		{129794, "AIS Class A Static and Voyage Related Data"},
		{129795, "AIS Addressed Binary Message"},
		{129796, "AIS Acknowledge"},
		{129797, "AIS Binary Broadcast Message"},
		{129798, "AIS SAR Aircraft Position Report"},
		{129799, "Radio Frequency / Mode / Power"},
		{129800, "AIS UTC / Date Enquiry"},
		{129801, "AIS Addressed Safety Related Message"},
		{129802, "AIS Safety Related Broadcast Message"},
		{129803, "AIS Interrogation"},
		{129804, "AIS Assignment Mode Command"},
		{129805, "AIS Data Link Management Message"},
		{129806, "AIS Channel Management"},
		{129807, "AIS Group Assignment"},
		{129808, "DSC Call Information"},
		{129809, "AIS Class B 'CS' Static Data Report, Part A"},
		{129810, "AIS Class B 'CS' Static Data Report, Part B"},
		{129811, "AIS Single Slot Binary Message"},
		{129812, "AIS Multi Slot Binary Message"},
		{129813, "AIS Long - Range Broadcast Message"},
		{130052, "Loran - C TD Data"},
		{130053, "Loran - C Range Data"},
		{130054, "Loran - C Signal Data"},
		{130060, "Label"},
		{130061, "Channel Source Configuration"},
		{130064, "Route and WP Service - Database List"},
		{130065, "Route and WP Service - Route List"},
		{130066, "Route and WP Service - Route/WP - List Attributes"},
		{130067, "Route and WP Service - Route - WP Name & Position"},
		{130068, "Route and WP Service - Route - WP - Name"},
		{130069, "Route and WP Service - XTE Limit & Navigation Method"},
		{130070, "Route and WP Service - WP Comment"},
		{130071, "Route and WP Service - Route Comment"},
		{130072, "Route and WP Service - Database Comment"},
		{130073, "Route and WP Service - Radius of Turn"},
		{130074, "Route and WP Service - WP List - WP Name & Position"},
		{130306, "Wind Data"},
		{130310, "Environmental Parameters - DEPRECATED"},
		{130311, "Environmental Parameters- DEPRECATED"},
		{130312, "Temperature - DEPRECATED"},
		{130313, "Humidity"},
		{130314, "Actual Pressure"},
		{130315, "Set Pressure"},
		{130316, "Temperature, Extended Range"},
		{130320, "Tide Station Data"},
		{130321, "Salinity Station Data"},
		{130322, "Current Station Data"},
		{130323, "Meteorological Station Data"},
		{130324, "Moored Buoy Station Data"},
		{130560, "Payload Mass"},
		{130567, "Watermaker Input Setting and Status"},
		{130576, "Trim Tab Status"},
		{130577, "Direction Data"},
		{130578, "Vessel Speed Components"},
		{130820, "Manufacturer Proprietary Fast Packet"},
		{130821, "Manufacturer Proprietary Fast Packet"},
		{130822, "Manufacturer Proprietary Fast Packet"},
		{130823, "Manufacturer Proprietary Fast Packet"},
		{130824, "Manufacturer Proprietary Fast Packet"},
		{130840, "Manufacturer Proprietary Fast Packet"},
		{130845, "Manufacturer Proprietary Fast Packet (Navico Backlight Level and Mode)"},
		{130850, "Manufacturer Proprietary Fast Packet (NAC-3 Autopilot Mode)" },
		{130851, "Manufacturer Proprietary Fast Packet (NAC-3 Autopilot Status)"},
		{130856, "Manufacturer Proprietary Fast Packet (NAC-3 Autopilot Alert Message)" } };

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

	// General Purpose parser
	wxString ParseMessage(std::vector<uint8_t> data);

	// BUG BUG Remove
	bool subscribeFlag = true;

};

#endif 