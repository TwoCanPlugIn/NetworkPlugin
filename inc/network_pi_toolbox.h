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
// NMEA2000� is a registered trademark of the National Marine Electronics Association

#ifndef NETWORK_TOOLBOX_H
#define NETWORK_TOOLBOX_H

// wxWidgets Precompiled Headers
#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif

// The dialog base class from which we are derived
// Note wxFormBuilder was used to generate the UI
#include "network_pi_toolboxbase.h"

// Flag used to indicate if any settings have been changed
extern bool settingsDirty;

class NetworkToolbox : public NetworkToolboxBase {
	
public:
	NetworkToolbox(wxWindow* parent);
	~NetworkToolbox();
	
	
protected:
	// Overridden methods from the base class
	void OnCountdownTimerChanged(wxSpinEvent& event);
	void OnTackingAngleChanged(wxSpinEvent& event);
	void OnWindChanged(wxCommandEvent& event);
	void OnHeadingChanged(wxCommandEvent& event);
	void OnLayLinesChanged(wxCommandEvent& event);
private:
	
};

#endif
