///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "network_pi_dialogbase.h"

///////////////////////////////////////////////////////////////////////////

NetworkDialogBase::NetworkDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* sizerDialog;
	sizerDialog = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* sizerTabs;
	sizerTabs = new wxBoxSizer( wxVERTICAL );

	gridNetwork = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	gridNetwork->CreateGrid( 253, 3 );
	gridNetwork->EnableEditing( false );
	gridNetwork->EnableGridLines( true );
	gridNetwork->EnableDragGridSize( false );
	gridNetwork->SetMargins( 0, 0 );

	// Columns
	gridNetwork->SetColSize( 0, 85 );
	gridNetwork->SetColSize( 1, 120 );
	gridNetwork->SetColSize( 2, 120 );
	gridNetwork->EnableDragColMove( false );
	gridNetwork->EnableDragColSize( true );
	gridNetwork->SetColLabelSize( 30 );
	gridNetwork->SetColLabelValue( 0, wxT("Unique Id") );
	gridNetwork->SetColLabelValue( 1, wxT("Manufacturer") );
	gridNetwork->SetColLabelValue( 2, wxT("Model Id") );
	gridNetwork->SetColLabelAlignment( wxALIGN_LEFT, wxALIGN_CENTER );

	// Rows
	gridNetwork->EnableDragRowSize( true );
	gridNetwork->SetRowLabelSize( 80 );
	gridNetwork->SetRowLabelAlignment( wxALIGN_LEFT, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	gridNetwork->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	sizerTabs->Add( gridNetwork, 0, wxALL, 5 );


	sizerDialog->Add( sizerTabs, 0, wxEXPAND, 5 );

	wxBoxSizer* sizerButtons;
	sizerButtons = new wxBoxSizer( wxHORIZONTAL );

	btnCancel = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerButtons->Add( btnCancel, 0, wxALL, 5 );

	btnOK = new wxButton( this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerButtons->Add( btnOK, 0, wxALL, 5 );


	sizerDialog->Add( sizerButtons, 0, wxEXPAND, 5 );


	this->SetSizer( sizerDialog );
	this->Layout();
	sizerDialog->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	gridNetwork->Connect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( NetworkDialogBase::OnRightClick ), NULL, this );
	btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnCancel ), NULL, this );
	btnOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnOk ), NULL, this );
}

NetworkDialogBase::~NetworkDialogBase()
{
	// Disconnect Events
	gridNetwork->Disconnect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( NetworkDialogBase::OnRightClick ), NULL, this );
	btnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnCancel ), NULL, this );
	btnOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnOk ), NULL, this );

}
