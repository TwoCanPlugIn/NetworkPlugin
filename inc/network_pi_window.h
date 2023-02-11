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

#ifndef NETWORK_WINDOW_H
#define NETWORK_WINDOW_H

// The dialog base class from which we are derived
// Note wxFormBuilder used to generate UI
#include "network_pi_windowbase.h"

// For the Stopwatch/Countdown timer
#include <wx/timer.h>

// BUG BUG DEBUG - Logging
#include <wx/log.h>

// image for dialog icon
extern wxBitmap *_img_network_colour;

extern const int NETWORKDIALOG_CLOSE_EVENT;
extern const int NETWORKDIALOG_OPEN_EVENT;
extern const int NETWORKDIALOG_PING_EVENT;
extern const wxEventType wxEVT_NETWORK_PLUGIN_EVENT;

extern bool networkWindowIsVisible;

class NetworkWindow : public NetworkWindowBase {
	
public:
	NetworkWindow(wxWindow* parent, wxEvtHandler *handler);
	~NetworkWindow();
	// wxTimer used as uptime timer 
	wxTimer *stopWatch;
	void OnTimer(wxTimerEvent& event);
	wxEvtHandler *eventHandlerAddress;
	
protected:
	//overridden methods from the base class
	void OnClose(wxCloseEvent& event);
	void OnReset(wxCommandEvent &event);
	void OnPing(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
		
private:
	void Initialize(void);
	void ResetTimer(void);
	// Network uptime
	int totalSeconds;
	int seconds;
	int minutes;
	
	// Parent Windows Size
	int parentWidth;
	int parentHeight;
};

#endif
