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
	wxBoxSizer* sizerPanel;
	sizerPanel = new wxBoxSizer( wxVERTICAL );

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
	sizerPanel->Add( gridNetwork, 0, wxALL, 5 );


	this->SetSizer( sizerPanel );
	this->Layout();

	// Connect Events
	this->Connect( wxEVT_AUI_PANE_ACTIVATED, wxAuiManagerEventHandler( NetworkDialogBase::OnActivate ) );
	this->Connect( wxEVT_AUI_PANE_CLOSE, wxAuiManagerEventHandler( NetworkDialogBase::OnClose ) );
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( NetworkDialogBase::OnInit ) );
	gridNetwork->Connect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( NetworkDialogBase::OnRightClick ), NULL, this );
}

NetworkDialogBase::~NetworkDialogBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_AUI_PANE_ACTIVATED, wxAuiManagerEventHandler( NetworkDialogBase::OnActivate ) );
	this->Disconnect( wxEVT_AUI_PANE_CLOSE, wxAuiManagerEventHandler( NetworkDialogBase::OnClose ) );
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( NetworkDialogBase::OnInit ) );
	gridNetwork->Disconnect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( NetworkDialogBase::OnRightClick ), NULL, this );

}
