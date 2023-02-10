///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jan 23 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __NETWORK_TOOLBOXBASE_H__
#define __NETWORK_TOOLBOXBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/panel.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class RacingToolboxbase
///////////////////////////////////////////////////////////////////////////////
class NetworkToolboxBase : public wxPanel 
{
	private:
	
	protected:
		wxStaticText* labelCountdownTimer;
		wxSpinCtrl* spinCountdownTimer;
		wxStaticText* labelTackingAngle;
		wxSpinCtrl* spinTackingAngle;
		wxCheckBox* chkWind;
		wxCheckBox* chkHeading;
		wxCheckBox* chkLayLines;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCountdownTimerChanged( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnTackingAngleChanged( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnWindChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnHeadingChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLayLinesChanged( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		NetworkToolboxBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,328 ), long style = wxTAB_TRAVERSAL ); 
		~NetworkToolboxBase();
	
};

#endif //__RACING_TOOLBOXBASE_H__
