///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/font.h>
#include <wx/grid.h>
#include <wx/gdicmn.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/aui/aui.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class panelNetwork
///////////////////////////////////////////////////////////////////////////////
class NetworkDialogBase : public wxPanel
{
	private:

	protected:
		wxGrid* gridNetwork;

		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxAuiManagerEvent& event ) { event.Skip(); }
		virtual void OnActivate( wxAuiManagerEvent& event ) { event.Skip(); }
		virtual void OnRightClick( wxGridEvent& event ) { event.Skip(); }


	public:

		NetworkDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~NetworkDialogBase();

};

