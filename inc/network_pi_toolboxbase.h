///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class NetworkToolboxBase
///////////////////////////////////////////////////////////////////////////////
class NetworkToolboxBase : public wxPanel
{
	private:

	protected:
		wxStaticText* labelInterface;
		wxChoice* cmbInterface;
		wxCheckBox* chkHeartbeat;
		wxStaticText* labelHeartbeat;
		wxSpinCtrl* spinInterval;
		wxCheckBox* chkNetwork;
		wxCheckBox* chkGarmin;
		wxCheckBox* chkNavico;

		// Virtual event handlers, override them in your derived class
		virtual void OnInit( wxInitDialogEvent& event ) { event.Skip(); }
		virtual void OnInterfaceChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnHeartbeatChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnIntervalChanged( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnInfoChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnGarminChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNavicoChanged( wxCommandEvent& event ) { event.Skip(); }


	public:

		NetworkToolboxBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,328 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );

		~NetworkToolboxBase();

};

