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


// Project: NMEA 2000 Network Plugin
// Description: modal dialog for OpenCPN
// Owner: twocanplugin@hotmail.com
// Date: 31/01/2023
// Version History: 
// 1.0 Initial Release
//

#include "network_pi_dialog.h"

// Constructor and destructor implementation
NetworkDialog::NetworkDialog(wxWindow* parent, wxEvtHandler *handler) : NetworkDialogBase(parent) {

	// Maintain a reference to the parent event handler
	eventHandler = handler;

	// Ensure the dialog is sized correctly	
	wxSize newSize = this->GetSize();
	gridNetwork->SetMinSize(wxSize(512, 20 * gridNetwork->GetDefaultRowSize()));
	gridNetwork->SetMaxSize(wxSize(-1, 20 * gridNetwork->GetDefaultRowSize()));
	Fit();

	// After we've fitted in everything adjust the dataGrid column widths
	int colWidth = (int)((gridNetwork->GetSize().GetWidth() - gridNetwork->GetRowLabelSize() - wxSystemSettings::GetMetric(wxSYS_VSCROLL_X, NULL)) / 3);
	gridNetwork->SetColSize(0, colWidth);
	gridNetwork->SetColSize(1, colWidth);
	gridNetwork->SetColSize(2, colWidth);

	for (int i = 0; i < 254; i++) {
		// Renumber row labels to match network address 0 - 253
		gridNetwork->SetRowLabelValue(i, std::to_string(i));
	}
}

NetworkDialog::~NetworkDialog() {
	// Nothing to do in the destructor
}

void NetworkDialog::OnActivate(wxAuiManagerEvent& event) {
	wxMessageBox("AUI Actiate, OnInit");

	// Populate the Data Grid

	for (int i = 0; i < 254; i++) {
		// No need to iterate over non-existent entries
		if ((networkInformation[i].deviceInformation.uniqueId > 0) || (strlen(networkInformation[i].productInformation.modelId) > 0) ) {
			gridNetwork->SetCellValue(i, 0, wxString::Format("%lu", networkInformation[i].deviceInformation.uniqueId));
			// Look up the manufacturer name
			//std::unordered_map<int, std::string>::iterator it = deviceManufacturers.find(networkInfoirmation[i].manufacturerId);
			//if (it != deviceManufacturers.end()) {
			//	gridNetwork->SetCellValue(i, 1, it->second);
			//}
			//else {
				gridNetwork->SetCellValue(i, 1, wxString::Format("%d", networkInformation[i].deviceInformation.manufacturerId));
			//}
			gridNetwork->SetCellValue(i, 2, wxString::Format("%s", networkInformation[i].productInformation.modelId));
			// We don't receive our own heartbeats so ignore our time stamp value
			//if (networkInformation[i].uniqueId != uniqueId) {
			wxGridCellAttr *attr;
			attr = new wxGridCellAttr;
			// Differentiate dead/alive devices
			attr->SetTextColour((wxDateTime::Now() > (networkInformation[i].timestamp + wxTimeSpan::Seconds(60))) ? *wxRED : *wxGREEN);
			gridNetwork->SetAttr(i, 0, attr);
			//}
		}
	}
	event.Skip();
}

void NetworkDialog::OnClose(wxAuiManagerEvent& event) {

	// Notify the parent we have closed, so that it can update its toolbar state
	isNetworkDialogVisible = FALSE;

	wxCommandEvent *closeEvent = new wxCommandEvent(wxEVT_NETWORK_PLUGIN_EVENT, NETWORKDIALOG_CLOSE_EVENT);
	wxQueueEvent(eventHandler, closeEvent);

	event.Skip();
}

void NetworkDialog::OnRightClick(wxCommandEvent &event) {
	wxMessageBox(wxString::Format("Selected: %d", event.GetSelection()));
}