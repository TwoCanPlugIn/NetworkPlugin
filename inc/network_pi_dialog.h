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

#ifndef NETWORK_DIALOG_H
#define NETWORK_DIALOG_H

// wxWidgets Precompiled Headers
#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif

// The dialog base class from which we are derived
// Note wxFormBuilder used to generate UI
#include "network_pi_dialogbase.h"

#include <wx/timer.h>

#include <wx/aui/aui.h>

// Typedefs
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
	unsigned char selfConfigure;
} DeviceInformation;

typedef struct ConfigurationInformation {
	wxString information1;
	wxString information2;
	wxString information3;
} ConfigurationInformation;

// Used  to store the data for the Network Map, combines elements from address claim & product information
typedef struct NetworkDevice {
	DeviceInformation deviceInformation;
	ProductInformation productInformation;
	ConfigurationInformation configurationInformation;
	wxDateTime timestamp; // Updated upon reception of heartbeat or address claim. Used to determine stale entries
} NetworkDevice;


// image for dialog icon
extern wxBitmap *_img_network_colour;

// Events that we pass to the plugin
extern int const wxEVT_NETWORK_PLUGIN_EVENT;
extern const int NETWORKDIALOG_OPEN_EVENT;
extern const int NETWORKDIALOG_CLOSE_EVENT;
extern const int NETWORKDIALOG_PING_EVENT;

// Array of network devices & their product and device information
extern std::vector<NetworkDevice> networkDevices;

class NetworkDialog : public NetworkDialogBase {
	
public:
	NetworkDialog(wxWindow* parent, wxEvtHandler *handler);
	~NetworkDialog();
	
protected:
	//overridden methods from the base class
	void OnActivate(wxAuiManagerEvent& event);
	void OnInit(wxInitDialogEvent& event);
	void OnRightClick(wxGridEvent& event);
	void OnSubscribe(wxCommandEvent& event); 
	void OnUpdate(wxCommandEvent& event);
	void OnNMEA(wxCommandEvent& event);
	void OnAddress(wxCommandEvent& event);
	void OnProduct(wxCommandEvent& event);
	

private:
	// Parent Window Size
	int parentWidth;
	int parentHeight;
	
	// The plugin event handler
	wxEvtHandler *eventHandler;
};

#endif
