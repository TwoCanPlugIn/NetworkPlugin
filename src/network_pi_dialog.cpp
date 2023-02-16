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
NetworkDialog::NetworkDialog(wxWindow* parent) : NetworkDialogBase(parent) {

	// Set the dialog's icon
	wxIcon icon;
	icon.CopyFromBitmap(*_img_network_colour);
	SetIcon(icon);
	parent->GetSize(&parentWidth, &parentHeight);

	// Maintain a reference to the parent event handler
	//eventHandlerAddress = handler;
}

NetworkDialog::~NetworkDialog() {
	// Nothing to do in the destructor
}

void NetworkDialog::OnInit(wxInitDialogEvent& event) {
	// Ensure the dialog is sized correctly	
	Fit();

	// And move to bottom right of the screen
	int dialogWidth;
	int dialogHeight;
	this->GetSize(&dialogWidth, &dialogHeight);
	this->SetPosition(wxPoint(parentWidth - dialogWidth, parentHeight - dialogHeight));

	// Initialize the timer
	oneMinuteTimer = new wxTimer();
	oneMinuteTimer->Connect(oneMinuteTimer->GetId(), wxEVT_TIMER, wxTimerEventHandler(NetworkDialog::OnTimer), NULL, this);
	//ResetTimer();

}

void NetworkDialog::OnClose(wxCloseEvent& event) {

	oneMinuteTimer->Disconnect(oneMinuteTimer->GetId(), wxEVT_TIMER, wxTimerEventHandler(NetworkDialog::OnTimer));

	if (oneMinuteTimer != nullptr) {
		delete oneMinuteTimer;
	}

	// Notify the parent we have closed, so that it can update its toolbar state
	//networkWindowIsVisible = false;

	//wxCommandEvent *closeEvent = new wxCommandEvent(wxEVT_NETWORK_PLUGIN_EVENT, NETWORKDIALOG_CLOSE_EVENT);
	//wxQueueEvent(eventHandlerAddress, closeEvent);

	event.Skip();
}


void NetworkDialog::OnTimer(wxTimerEvent& event) {
	// Update the countdown timer
	totalSeconds -= 1;

	minutes = trunc(totalSeconds / 60);
	seconds = totalSeconds - (minutes * 60);

	// Display the count down timer
	//labelTimer->SetLabel(wxString::Format("%1d:%02d", minutes, seconds));

}

void NetworkDialog::OnPing(wxCommandEvent &event) {
	// Notify Parent
	//wxCommandEvent *closeEvent = new wxCommandEvent(wxEVT_NETWORK_PLUGIN_EVENT, NETWORKDIALOG_PING_EVENT);
	//wxQueueEvent(eventHandlerAddress, closeEvent);
}

void NetworkDialog::OnCancel(wxCommandEvent &event) {
	this->Close();
}

void NetworkDialog::ResetTimer(void) {
	// Reset the timer
	totalSeconds = 300;
	minutes = trunc(totalSeconds / 60);
	seconds = totalSeconds - (minutes * 60);
	//labelTimer->SetLabel(wxString::Format("%1d:%02d", minutes, seconds));
}


