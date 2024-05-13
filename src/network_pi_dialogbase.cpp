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

	buttonSubscribe = new wxButton( this, ID_SUBSCRIBE, wxT("(Un)Subscribe"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( buttonSubscribe, 1, wxALL, 5 );

	buttonUpdate = new wxButton( this, ID_UPDATE, wxT("Update"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( buttonUpdate, 1, wxALL, 5 );

	buttonNMEA = new wxButton( this, ID_NMEA, wxT("NMEA 0183"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( buttonNMEA, 0, wxALL, 5 );

	butonAddress = new wxButton( this, ID_ADDRESS, wxT("Rqst 60928"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( butonAddress, 0, wxALL, 5 );

	buttonProduct = new wxButton( this, ID_PRODUCT, wxT("Rqst 126996"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( buttonProduct, 0, wxALL, 5 );


	sizerPanel->Add( bSizer4, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer41;
	bSizer41 = new wxBoxSizer( wxVERTICAL );

	editBox = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer41->Add( editBox, 0, wxALL|wxEXPAND, 5 );


	sizerPanel->Add( bSizer41, 1, wxEXPAND, 5 );

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
	buttonSubscribe->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnSubscribe ), NULL, this );
	buttonUpdate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnUpdate ), NULL, this );
	buttonNMEA->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnNMEA ), NULL, this );
	butonAddress->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnAddress ), NULL, this );
	buttonProduct->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnProduct ), NULL, this );
	gridNetwork->Connect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( NetworkDialogBase::OnRightClick ), NULL, this );
}

NetworkDialogBase::~NetworkDialogBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( NetworkDialogBase::OnInit ) );
	buttonSubscribe->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnSubscribe ), NULL, this );
	buttonUpdate->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnUpdate ), NULL, this );
	buttonNMEA->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnNMEA ), NULL, this );
	butonAddress->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnAddress ), NULL, this );
	buttonProduct->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NetworkDialogBase::OnProduct ), NULL, this );
	gridNetwork->Disconnect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( NetworkDialogBase::OnRightClick ), NULL, this );

}
