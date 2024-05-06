///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/button.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/grid.h>
#include <wx/panel.h>

///////////////////////////////////////////////////////////////////////////

constexpr int wxID_SUBSCRIBE = wxID_HIGHEST + 1;
constexpr int wxID_NMEA = wxID_HIGHEST + 2;
constexpr int wxID_UPDATE = wxID_HIGHEST + 3;
constexpr int wxID_PRODUCT = wxID_HIGHEST + 4;
constexpr int wxID_ADDRESS = wxID_HIGHEST + 5;


///////////////////////////////////////////////////////////////////////////////
/// Class NetworkDialogBase
///////////////////////////////////////////////////////////////////////////////
class NetworkDialogBase : public wxPanel
{
	private:

	protected:
		wxButton* buttonSubscribe;
		wxButton* buttonUpdate;
		wxButton* buttonNMEA;
		wxButton* butonAddress;
		wxButton* buttonProduct;
		wxGrid* gridNetwork;

		// Virtual event handlers, override them in your derived class
		virtual void OnInit( wxInitDialogEvent& event ) { event.Skip(); }
		virtual void OnSubscribe( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNMEA( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAddress( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnProduct( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRightClick( wxGridEvent& event ) { event.Skip(); }


	public:

		NetworkDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 480,526 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );

		~NetworkDialogBase();

};

