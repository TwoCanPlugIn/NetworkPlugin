///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jan 23 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "network_pi_toolboxbase.h"

///////////////////////////////////////////////////////////////////////////

NetworkToolboxBase::NetworkToolboxBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	labelCountdownTimer = new wxStaticText( this, wxID_ANY, wxT("Countdown Time (minutes)"), wxDefaultPosition, wxDefaultSize, 0 );
	labelCountdownTimer->Wrap( -1 );
	bSizer1->Add( labelCountdownTimer, 0, wxALL, 5 );
	
	spinCountdownTimer = new wxSpinCtrl( this, wxID_ANY, wxT("5"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 20, 0 );
	bSizer1->Add( spinCountdownTimer, 0, wxALL, 5 );
	
	labelTackingAngle = new wxStaticText( this, wxID_ANY, wxT("Tacking Angle (degrees)"), wxDefaultPosition, wxDefaultSize, 0 );
	labelTackingAngle->Wrap( -1 );
	bSizer1->Add( labelTackingAngle, 0, wxALL, 5 );
	
	spinTackingAngle = new wxSpinCtrl( this, wxID_ANY, wxT("90"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 60, 180, 0 );
	bSizer1->Add( spinTackingAngle, 0, wxALL, 5 );
	
	chkWind = new wxCheckBox( this, wxID_ANY, wxT("Show Wind Direction"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( chkWind, 0, wxALL, 5 );
	
	chkHeading = new wxCheckBox( this, wxID_ANY, wxT("Show Heading"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( chkHeading, 0, wxALL, 5 );
	
	chkLayLines = new wxCheckBox( this, wxID_ANY, wxT("Show Lay Lines"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( chkLayLines, 0, wxALL, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect Events
	spinCountdownTimer->Connect(wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(NetworkToolboxBase::OnCountdownTimerChanged), NULL, this);
	spinTackingAngle->Connect(wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(NetworkToolboxBase::OnTackingAngleChanged), NULL, this);
	chkWind->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(NetworkToolboxBase::OnWindChanged), NULL, this);
	chkHeading->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(NetworkToolboxBase::OnHeadingChanged), NULL, this);
	chkLayLines->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( NetworkToolboxBase::OnLayLinesChanged ), NULL, this );
}

NetworkToolboxBase::~NetworkToolboxBase()
{
	// Disconnect Events
	spinCountdownTimer->Disconnect(wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(NetworkToolboxBase::OnCountdownTimerChanged), NULL, this);
	spinTackingAngle->Disconnect(wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(NetworkToolboxBase::OnTackingAngleChanged), NULL, this);
	chkWind->Disconnect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(NetworkToolboxBase::OnWindChanged), NULL, this);
	chkHeading->Disconnect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(NetworkToolboxBase::OnHeadingChanged), NULL, this);
	chkLayLines->Disconnect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(NetworkToolboxBase::OnLayLinesChanged), NULL, this);
	
}
