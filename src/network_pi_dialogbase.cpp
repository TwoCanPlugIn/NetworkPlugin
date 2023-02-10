///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "network_pi_dialogbase.h"

///////////////////////////////////////////////////////////////////////////

NetworkDialogBase::NetworkDialogBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* sizerDialog;
	sizerDialog = new wxBoxSizer( wxVERTICAL );

	wxGridSizer* sizerLabels;
	sizerLabels = new wxGridSizer( 2, 2, 0, 0 );

	labelSpeed = new wxStaticText( this, wxID_ANY, wxT("Speed"), wxDefaultPosition, wxDefaultSize, 0 );
	labelSpeed->SetLabelMarkup( wxT("Speed") );
	labelSpeed->Wrap( -1 );
	sizerLabels->Add( labelSpeed, 0, wxALL, 5 );

	labelTimer = new wxStaticText( this, wxID_ANY, wxT("Timer"), wxDefaultPosition, wxDefaultSize, 0 );
	labelTimer->SetLabelMarkup( wxT("Timer") );
	labelTimer->Wrap( -1 );
	sizerLabels->Add( labelTimer, 0, wxALL, 5 );

	labelTTG = new wxStaticText( this, wxID_ANY, wxT("TTG"), wxDefaultPosition, wxDefaultSize, 0 );
	labelTTG->SetLabelMarkup( wxT("TTG") );
	labelTTG->Wrap( -1 );
	sizerLabels->Add( labelTTG, 0, wxALL, 5 );

	labelDistance = new wxStaticText( this, wxID_ANY, wxT("Distance"), wxDefaultPosition, wxDefaultSize, 0 );
	labelDistance->SetLabelMarkup( wxT("Distance") );
	labelDistance->Wrap( -1 );
	sizerLabels->Add( labelDistance, 0, wxALL, 5 );


	sizerDialog->Add( sizerLabels, 1, wxEXPAND, 5 );

	wxGridSizer* sizerButtons;
	sizerButtons = new wxGridSizer( 2, 2, 0, 0 );

	buttonStart = new wxButton( this, wxID_ANY, wxT("Start Timer"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonStart->SetLabelMarkup( wxT("Start Timer") );
	sizerButtons->Add( buttonStart, 0, wxALL, 5 );

	buttonReset = new wxButton( this, wxID_ANY, wxT("Reset Timer"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonReset->SetLabelMarkup( wxT("Reset Timer") );
	sizerButtons->Add( buttonReset, 0, wxALL, 5 );

	buttonPort = new wxButton( this, wxID_ANY, wxT("Port Mark"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonPort->SetLabelMarkup( wxT("Port Mark") );
	sizerButtons->Add( buttonPort, 0, wxALL, 5 );

	buttonStbd = new wxButton( this, wxID_ANY, wxT("Stbd Mark"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonStbd->SetLabelMarkup( wxT("Stbd Mark") );
	sizerButtons->Add( buttonStbd, 0, wxALL, 5 );


	sizerDialog->Add( sizerButtons, 1, wxEXPAND, 5 );

	sizerDialogButtons = new wxStdDialogButtonSizer();
	sizerDialogButtonsCancel = new wxButton( this, wxID_CANCEL );
	sizerDialogButtons->AddButton( sizerDialogButtonsCancel );
	sizerDialogButtons->Realize();

	sizerDialog->Add( sizerDialogButtons, 1, wxEXPAND, 5 );


	this->SetSizer( sizerDialog );
	this->Layout();

	// Connect Events
	buttonStart->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnStart ), NULL, this );
	buttonReset->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnReset ), NULL, this );
	buttonPort->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnPort ), NULL, this );
	buttonStbd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnStbd ), NULL, this );
	sizerDialogButtonsCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnCancel ), NULL, this );
}

NetworkDialogBase::~NetworkDialogBase()
{
	// Disconnect Events
	buttonStart->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnStart ), NULL, this );
	buttonReset->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnReset ), NULL, this );
	buttonPort->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnPort ), NULL, this );
	buttonStbd->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnStbd ), NULL, this );
	sizerDialogButtonsCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnCancel ), NULL, this );

}
