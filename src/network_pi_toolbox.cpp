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


// Project: NMEA 2000 Network Plugin
// Description: Display NMEA 2000 Nrtwork information for OpenCPN
// Owner: twocanplugin@hotmail.com
// Date: 31/01/2023
// Version History: 
// 1.0 Initial Release
//

#include "network_pi_toolbox.h"

// Constructor and destructor implementation
NetworkToolbox::NetworkToolbox( wxWindow* parent) : NetworkToolboxBase(parent) {
	// BUG BUG This could go in an OnInit event ??
	settingsDirty = FALSE;
	ListInterfaces();
}

NetworkToolbox::~NetworkToolbox() {
	// Nothing to do in the destructor
}

void NetworkToolbox::ListInterfaces(void) {
	// Setup the NMEA 2000 Network interface
	activeDrivers.clear();
	activeDrivers = GetActiveDrivers();

	// Enumerate the drivers and select a NMEA 2000 network connection
	for (auto const& activeDriver : activeDrivers) {
		for (auto const& driver : GetAttributes(activeDriver)) {
			wxLogMessage(_T("Network Plugin, Interface: %s, Handle: %s, Protocol: %s"),
				activeDriver, driver.first, driver.second);
			if (driver.second == "NMEA2000") {
				cmbInterface->Append(activeDriver);
				//if (driverHandle == activeDriver) {
				//	cmbInterface->SetStringSelection(activeDriver);
				//}
				// BUG BUG Need to prettify the display. 
				// This is just stupid. nmea2000!@!COM6
			}
		}
	}
}

void NetworkToolbox::OnInterfaceSelected(wxCommandEvent& event) {
	settingsDirty = TRUE;
}

void NetworkToolbox::OnIntervaChanged(wxSpinEvent& event) {
	settingsDirty = TRUE;
}

void NetworkToolbox::OnHeartbeatChanged(wxCommandEvent& event) {
	settingsDirty = TRUE;
}

void NetworkToolbox::OnNetworkChanged(wxCommandEvent& event) {
	settingsDirty = TRUE;
}

wxString NetworkToolbox::GetInterface(void) {
	return cmbInterface->GetString(cmbInterface->GetSelection());
}


void NetworkToolbox::SetInterface(wxString interfaceName) {
	cmbInterface->SetSelection(cmbInterface->FindString(interfaceName));
}

void NetworkToolbox::SetHeartbeat(bool heartbeatValue) {
	chkHeartbeat->SetValue(heartbeatValue);
}

void NetworkToolbox::SetNetwork(bool networkValue) {
	chkNetwork->SetValue(networkValue);
}

void NetworkToolbox::SetInterval(int intervalValue) {
	spinInterval->SetValue(intervalValue);
}


bool NetworkToolbox::GetHeartbeat(void) {
	return chkHeartbeat->IsChecked();
}

bool NetworkToolbox::GetNetwork(void) {
	return chkNetwork->IsChecked();
}

int NetworkToolbox::GetInterval(void) {
	return spinInterval->GetValue();
}