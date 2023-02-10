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

#include <wx/log.h>

// BUG BUG
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))


// image for dialog icon
extern wxBitmap *_img_racing_logo_32;

class NetworkDialog : public NetworkDialogBase {
	
public:
	NetworkDialog(wxWindow* parent);
	~NetworkDialog();
	// wxTimer used to update network view 
	wxTimer *oneMinuteTimer;
	void OnTimer(wxTimerEvent& event);
	
	
protected:
	//overridden methods from the base class
	void OnInit(wxInitDialogEvent& event);
	void OnClose(wxCloseEvent& event);
	void OnPing(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
	
private:
	void ResetTimer(void);
	// System Uptime ??
	int totalSeconds;
	int seconds;
	int minutes;
	
	// Parent Windows Size
	int parentWidth;
	int parentHeight;
};

#endif
