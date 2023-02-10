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


// Project: NMEA 2000 Network Plugin
// Description: NMEA 2000 Network Display plugin for OpenCPN
// Owner: twocanplugin@hotmail.com
// Date: 31/01/2023
// Version History: 
// 1.0 Initial Release
//

#include "network_pi_dialog.h"

// Constructor and destructor implementation
NetworkDialog::NetworkDialog( wxWindow* parent) : NetworkDialogBase(parent) {
	// Set the dialog's icon
	wxIcon icon;
	icon.CopyFromBitmap(*_img_racing_logo_32);
	parent->GetSize(&parentWidth, &parentHeight);
}

NetworkDialog::~NetworkDialog() {
	// Nothing to do in the destructor
}

void NetworkDialog::OnInit(wxInitDialogEvent& event) {
	// Set larger and more readable fonts
	wxFont bigFont = labelSpeed->GetFont();
	bigFont.SetPointSize( 16 );
	bigFont.SetWeight(wxFONTWEIGHT_BOLD);
	labelSpeed->SetFont( bigFont );
	labelTimer->SetFont( bigFont );
	labelDistance->SetFont( bigFont );
	labelTTG->SetFont( bigFont );
		
	// Ensure the dialog is sized correctly	
	Fit();
		
	// And move to bottom right of the screen
	// BUG BUG OnResize ??
	int dialogWidth;
	int dialogHeight;
	this->GetSize(&dialogWidth, &dialogHeight);
	this->SetPosition(wxPoint(parentWidth-dialogWidth,parentHeight - dialogHeight));
	
	// Initialize the timer
	oneMinuteTimer = new wxTimer();
	oneMinuteTimer->Connect(oneMinuteTimer->GetId(), wxEVT_TIMER, wxTimerEventHandler(NetworkDialog::OnTimer), NULL, this);
	ResetTimer();

}

void NetworkDialog::OnClose(wxCloseEvent& event) {
	if (oneMinuteTimer->IsRunning()) {
		oneMinuteTimer->Stop();
	}
	oneMinuteTimer->Disconnect(oneMinuteTimer->GetId(), wxEVT_TIMER, wxTimerEventHandler(NetworkDialog::OnTimer));
	delete oneMinuteTimer;

	Close();
}

void NetworkDialog::OnTimer(wxTimerEvent& event) {
	// Update the countdown timer
	totalSeconds -= 1;
	
	minutes = trunc(totalSeconds / 60);
	seconds = totalSeconds - (minutes * 60);
	
	// Display the count down timer
	labelTimer->SetLabel(wxString::Format("%1d:%02d",minutes, seconds));

	// Display our current speed
	// BUG BUG Check what OpenCPN sends, knots or metres per second....
	//labelSpeed->SetLabel(wxString::Format("%02.2f Kts",speedOverGround));
	
	
}

void NetworkDialog::OnCancel(wxCommandEvent &event) {
	if (oneMinuteTimer->IsRunning()) {
		oneMinuteTimer->Stop();
	}
	oneMinuteTimer->Disconnect(oneMinuteTimer->GetId(), wxEVT_TIMER, wxTimerEventHandler(NetworkDialog::OnTimer));
	delete oneMinuteTimer;

	Close();
}

void NetworkDialog::ResetTimer(void) {
	// Reset the countdown timer for 5 minutes
	totalSeconds = 300;
	minutes = trunc(totalSeconds / 60);
	seconds = totalSeconds - (minutes * 60);
	labelTimer->SetLabel(wxString::Format("%1d:%02d",minutes, seconds));
}