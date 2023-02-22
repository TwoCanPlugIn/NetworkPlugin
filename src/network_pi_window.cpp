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
// Description: non-modal dialog for OpenCPN
// Owner: twocanplugin@hotmail.com
// Date: 31/01/2023
// Version History: 
// 1.0 Initial Release
//

#include "network_pi_window.h"

// Constructor and destructor implementation
NetworkWindow::NetworkWindow( wxWindow* parent, wxEvtHandler *handler) : NetworkWindowBase(parent) {
	
	// Set the dialog's icon
	wxIcon icon;
	icon.CopyFromBitmap(*_img_network_colour);
	NetworkWindow::SetIcon(icon);
	parent->GetSize(&parentWidth, &parentHeight);

	// Maintain a reference to the parent event handler
	eventHandlerAddress = handler;

	Initialize();
}

NetworkWindow::~NetworkWindow() {
	// Nothing to do in the destructor
}

void NetworkWindow::Initialize(void) {
	// Set larger and more readable fonts
	wxFont bigFont = labelSpeed->GetFont();
	bigFont.SetPointSize( 20 );
	bigFont.SetWeight(wxFONTWEIGHT_BOLD);
	labelSpeed->SetFont( bigFont );
	labelTimer->SetFont( bigFont );
	labelDistance->SetFont( bigFont );
	labelTTG->SetFont( bigFont );
		
	// Ensure the dialog is sized correctly	
	Fit();

	// And move to bottom right of the screen
	int dialogWidth;
	int dialogHeight;
	this->GetSize(&dialogWidth, &dialogHeight);
	this->SetPosition(wxPoint(parentWidth-dialogWidth, parentHeight - dialogHeight));
		
	// Initialize the timer
	stopWatch = new wxTimer();
	stopWatch->Connect(stopWatch->GetId(), wxEVT_TIMER, wxTimerEventHandler(NetworkWindow::OnTimer), NULL, this);
	ResetTimer();

}

void NetworkWindow::OnClose(wxCloseEvent& event) {
	if (stopWatch->IsRunning()) {
		stopWatch->Stop();
	}

	stopWatch->Disconnect(stopWatch->GetId(), wxEVT_TIMER, wxTimerEventHandler(NetworkWindow::OnTimer));

	if (stopWatch != nullptr) {
		delete stopWatch;
	}

	// Notify the parent we have closed, so that it can update its toolbar state
	isNetworkDialogVisible = false;

	wxCommandEvent *closeEvent = new wxCommandEvent(wxEVT_NETWORK_PLUGIN_EVENT, NETWORKDIALOG_CLOSE_EVENT);
	wxQueueEvent(eventHandlerAddress, closeEvent);

	event.Skip();
}


void NetworkWindow::OnTimer(wxTimerEvent& event) {
	// Update the countdown timer
	totalSeconds -= 1;
	
	minutes = trunc(totalSeconds / 60);
	seconds = totalSeconds - (minutes * 60);
	
	// Display the count down timer
	labelTimer->SetLabel(wxString::Format("%1d:%02d",minutes, seconds));
		
}

void NetworkWindow::OnReset(wxCommandEvent &event) {
	if (stopWatch->IsRunning()) {
		stopWatch->Stop();
	}
	ResetTimer();
}

void NetworkWindow::OnPing(wxCommandEvent &event) {
	// Notify Parent
	wxCommandEvent *closeEvent = new wxCommandEvent(wxEVT_NETWORK_PLUGIN_EVENT, NETWORKDIALOG_PING_EVENT);
	wxQueueEvent(eventHandlerAddress, closeEvent);
}

void NetworkWindow::OnCancel(wxCommandEvent &event) {
	this->Close();
}

void NetworkWindow::ResetTimer(void) {
	// Reset the countdown timer for 5 minutes
	totalSeconds = 300;
	minutes = trunc(totalSeconds / 60);
	seconds = totalSeconds - (minutes * 60);
	labelTimer->SetLabel(wxString::Format("%1d:%02d",minutes, seconds));
}


