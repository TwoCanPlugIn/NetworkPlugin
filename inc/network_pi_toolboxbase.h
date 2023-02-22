///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
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
#include <wx/combobox.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>
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
		wxComboBox* cmbInterfce;
		wxStaticText* labelHeartbeat;
		wxSpinCtrl* spinInterval;
		wxCheckBox* chkHeartbeat;
		wxCheckBox* chkNetwork;

		// Virtual event handlers, overide them in your derived class
		virtual void OnInterfaceSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnIntervaChanged( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnHeartbeatChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNetworkChanged( wxCommandEvent& event ) { event.Skip(); }


	public:

		NetworkToolboxBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,328 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~NetworkToolboxBase();

};

