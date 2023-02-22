///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "network_pi_toolboxbase.h"

///////////////////////////////////////////////////////////////////////////

NetworkToolboxBase::NetworkToolboxBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* sizerPanel;
	sizerPanel = new wxBoxSizer( wxVERTICAL );

	labelInterface = new wxStaticText( this, wxID_ANY, wxT("NMEA 2000 Interface"), wxDefaultPosition, wxDefaultSize, 0 );
	labelInterface->Wrap( -1 );
	sizerPanel->Add( labelInterface, 0, wxALL, 5 );

	cmbInterfce = new wxComboBox( this, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	sizerPanel->Add( cmbInterfce, 0, wxALL, 5 );

	labelHeartbeat = new wxStaticText( this, wxID_ANY, wxT("Heartbeat Interval"), wxDefaultPosition, wxDefaultSize, 0 );
	labelHeartbeat->Wrap( -1 );
	sizerPanel->Add( labelHeartbeat, 0, wxALL, 5 );

	spinInterval = new wxSpinCtrl( this, wxID_ANY, wxT("1"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 5, 1 );
	sizerPanel->Add( spinInterval, 0, wxALL, 5 );

	chkHeartbeat = new wxCheckBox( this, wxID_ANY, wxT("Send Heartbeats"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerPanel->Add( chkHeartbeat, 0, wxALL, 5 );

	chkNetwork = new wxCheckBox( this, wxID_ANY, wxT("Request Network Information"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerPanel->Add( chkNetwork, 0, wxALL, 5 );


	this->SetSizer( sizerPanel );
	this->Layout();

	// Connect Events
	cmbInterfce->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( NetworkToolboxBase::OnInterfaceSelected ), NULL, this );
	spinInterval->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( NetworkToolboxBase::OnIntervaChanged ), NULL, this );
	chkHeartbeat->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( NetworkToolboxBase::OnHeartbeatChanged ), NULL, this );
	chkNetwork->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( NetworkToolboxBase::OnNetworkChanged ), NULL, this );
}

NetworkToolboxBase::~NetworkToolboxBase()
{
	// Disconnect Events
	cmbInterfce->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( NetworkToolboxBase::OnInterfaceSelected ), NULL, this );
	spinInterval->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( NetworkToolboxBase::OnIntervaChanged ), NULL, this );
	chkHeartbeat->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( NetworkToolboxBase::OnHeartbeatChanged ), NULL, this );
	chkNetwork->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( NetworkToolboxBase::OnNetworkChanged ), NULL, this );

}
