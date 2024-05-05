///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "network_pi_dialogbase.h"

///////////////////////////////////////////////////////////////////////////

NetworkDialogBase::NetworkDialogBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* sizerPanel;
	sizerPanel = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );


	bSizer4->Add( 0, 0, 4, wxEXPAND, 5 );

	buttonB = new wxButton( this, wxID_ANY, wxT("B"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( buttonB, 1, wxALL, 5 );

	buttonA = new wxButton( this, wxID_ANY, wxT("A"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( buttonA, 1, wxALL, 5 );


	sizerPanel->Add( bSizer4, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );

	gridNetwork = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	gridNetwork->CreateGrid( 243, 3 );
	gridNetwork->EnableEditing( true );
	gridNetwork->EnableGridLines( true );
	gridNetwork->EnableDragGridSize( false );
	gridNetwork->SetMargins( 0, 0 );

	// Columns
	gridNetwork->EnableDragColMove( false );
	gridNetwork->EnableDragColSize( true );
	gridNetwork->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	gridNetwork->EnableDragRowSize( true );
	gridNetwork->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	gridNetwork->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	bSizer5->Add( gridNetwork, 0, wxALL|wxEXPAND, 5 );


	sizerPanel->Add( bSizer5, 1, wxEXPAND, 5 );


	this->SetSizer( sizerPanel );
	this->Layout();

	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( NetworkDialogBase::OnInit ) );
	buttonB->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnButtonA ), NULL, this );
	buttonA->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnButtonB ), NULL, this );
	gridNetwork->Connect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( NetworkDialogBase::OnRightClick ), NULL, this );
}

NetworkDialogBase::~NetworkDialogBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( NetworkDialogBase::OnInit ) );
	buttonB->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnButtonA ), NULL, this );
	buttonA->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnButtonB ), NULL, this );
	gridNetwork->Disconnect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( NetworkDialogBase::OnRightClick ), NULL, this );

}
