#include "sampledatadlg.h"
#include "wx/stdpaths.h"
#include "wx/wx.h"

IMPLEMENT_DYNAMIC_CLASS( SampleDataDlg, wxDialog )

BEGIN_EVENT_TABLE( SampleDataDlg, wxDialog )
    EVT_CLOSE( SampleDataDlg::OnClose )
    EVT_BUTTON( ID_BUTTON_OK, SampleDataDlg::OnOK )
    EVT_BUTTON( ID_BUTTON_BROWSE, SampleDataDlg::OnBrowse )
	EVT_BUTTON( ID_BUTTON_CLEAR, SampleDataDlg::OnClear )
	EVT_SPINCTRL( ID_SPIN_KEYBOARDNOTE, SampleDataDlg::UpdateSemitones )
	EVT_SPINCTRL( ID_SPIN_SAMPLENOTE, SampleDataDlg::UpdateSemitones )
	EVT_TEXT( ID_SPIN_KEYBOARDNOTE, SampleDataDlg::UpdateSemitones )
	EVT_TEXT( ID_SPIN_SAMPLENOTE, SampleDataDlg::UpdateSemitones )
END_EVENT_TABLE()

SampleDataDlg::SampleDataDlg( )
{
}

SampleDataDlg::~SampleDataDlg()
{
}

SampleDataDlg::SampleDataDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

bool SampleDataDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    // Set member variables here
    _textColour.Set( 150, 150, 250 );
    _backgroundColour.Set( 0, 8, 40 );
    _spnSampleNote = NULL;
    _spnKeyboardNote = NULL;
    _txtFilename = NULL;
    _spnVolume = NULL;
    _btnBrowse = NULL;
    _btnClose = NULL;
	_btnClear = NULL;
	_resampleSemitones = NULL;

    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
    return true;
}

void SampleDataDlg::CreateControls()
{    
    SampleDataDlg* itemDialog1 = this;

	wxSize spinTextSize = wxSize(64, -1);

    this->SetBackgroundColour(_backgroundColour);

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxFlexGridSizer* itemBoxSizer3 = new wxFlexGridSizer(5, 2, 0, 0);
    itemBoxSizer2->Add(itemBoxSizer3, 0, 0, 0 );

    wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("Sample Note:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText3->SetForegroundColour(_textColour);
    itemBoxSizer3->Add(itemStaticText3, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    _spnSampleNote = new wxSpinCtrl( itemDialog1, ID_SPIN_SAMPLENOTE, _T("64"), wxDefaultPosition, spinTextSize, wxSP_ARROW_KEYS, 24, 107, 64 );
    itemBoxSizer3->Add(_spnSampleNote, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Keyboard Note:"), wxDefaultPosition, wxDefaultSize );
    itemStaticText4->SetForegroundColour(_textColour);
    itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

	wxBoxSizer* itemBoxSizer26 = new wxBoxSizer(wxHORIZONTAL);

	_spnKeyboardNote = new wxSpinCtrl( itemDialog1, ID_SPIN_KEYBOARDNOTE, _T("64"), wxDefaultPosition, spinTextSize, wxSP_ARROW_KEYS, 24, 107, 64 );
	_spnKeyboardNote->Enable(false);
    itemBoxSizer26->Add(_spnKeyboardNote, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxStaticText* itemStaticText78 = new wxStaticText( itemDialog1, wxID_STATIC, _("Resample:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemStaticText78->SetForegroundColour(_textColour);
	itemBoxSizer26->Add(itemStaticText78, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	_resampleSemitones = new wxStaticText( itemDialog1, wxID_STATIC, _("+0"), wxDefaultPosition, wxSize(24,-1), 0 );
	_resampleSemitones->SetForegroundColour(_textColour);
	itemBoxSizer26->Add(_resampleSemitones, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer3->Add(itemBoxSizer26, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 0 );

    wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Volume:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText5->SetForegroundColour(_textColour);
    itemBoxSizer3->Add(itemStaticText5, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    _spnVolume = new wxSpinCtrl( itemDialog1, ID_SPIN_VOLUME, _T("127"), wxDefaultPosition, spinTextSize, wxSP_ARROW_KEYS, 0, 127, 127 );
    itemBoxSizer3->Add(_spnVolume, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText55 = new wxStaticText( itemDialog1, wxID_STATIC, _("Loop Sample:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText55->SetForegroundColour(_textColour);
    itemBoxSizer3->Add(itemStaticText55, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

	_chkLoop = new wxCheckBox( itemDialog1, ID_CHECK_LOOP, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer3->Add(_chkLoop, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5 );

    wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("Filename:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText6->SetForegroundColour(_textColour);
    itemBoxSizer3->Add(itemStaticText6, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);

    _txtFilename = new wxTextCtrl( itemDialog1, ID_TEXT_FILENAME, _T(""), wxDefaultPosition, wxSize(200, -1), 0 );
    itemBoxSizer8->Add(_txtFilename, 0, wxGROW|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    _btnBrowse = new wxButton( itemDialog1, ID_BUTTON_BROWSE, _T("Browse" ));
    itemBoxSizer8->Add(_btnBrowse, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    itemBoxSizer3->Add(itemBoxSizer8, 0, 0, 0 );

    wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer7, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 0 );

    _btnClear = new wxButton( itemDialog1, ID_BUTTON_CLEAR, _T("Clear" ));
    itemBoxSizer7->Add(_btnClear, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    _btnClose = new wxButton( itemDialog1, ID_BUTTON_OK, _T("OK" ));
    itemBoxSizer7->Add(_btnClose, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

}

/**
* Updates the indicator that tells us how far the sample will be resampled.
*/
void SampleDataDlg::UpdateSemitones()
{
	int sample = _spnSampleNote->GetValue();
	int key = _spnKeyboardNote->GetValue();
	int diff = key - sample;
	if( diff < 0 )
		_resampleSemitones->SetLabel(wxString::Format(_("%d"), diff ));
	else
		_resampleSemitones->SetLabel(wxString::Format(_("+%d"), diff ));
}

void SampleDataDlg::UpdateSemitones(wxCommandEvent&)
{
	UpdateSemitones();
}

void SampleDataDlg::UpdateSemitones(wxSpinEvent&)
{
	UpdateSemitones();
}

void SampleDataDlg::OnOK( wxCommandEvent& event )
{
	EndModal(0);
    event.Skip();
}

void SampleDataDlg::OnClose( wxCloseEvent& event )
{
    EndModal(0);
    event.Skip();
}

/**
* Populates the dialog fields back into the active sample data.
*/
SampleData* SampleDataDlg::GetSample()
{
	SampleData* sample = new SampleData;
	sample->_sampleNote = _spnSampleNote->GetValue();
	sample->_volume = _spnVolume->GetValue();
	sample->_filename = _txtFilename->GetValue();
	sample->_keyboardNote = _spnKeyboardNote->GetValue();
	sample->_looping = _chkLoop->IsChecked();
	return sample;
}

/**
* Sets the sample data to be altered by the dialog when shown.
*/
void SampleDataDlg::SetSample( SampleData* sample )
{
    _spnKeyboardNote->SetValue( wxString::Format( _("%d"), sample->_keyboardNote ));
    _spnVolume->SetValue( wxString::Format( _("%d"), sample->_volume ));
	if( sample->_userSpecified )
	{
		_txtFilename->SetValue( sample->_filename );
		_spnSampleNote->SetValue( wxString::Format( _("%d"), sample->_sampleNote ));
	}
	else
	{
		_txtFilename->SetValue(_(""));
	    _spnSampleNote->SetValue( wxString::Format( _("%d"), sample->_keyboardNote ));
	}
	_chkLoop->SetValue(sample->_looping);
}

/**
* Lets user pick a sample to be used for a particular key on the keyboard.
*/
void SampleDataDlg::OnBrowse( wxCommandEvent& event )
{
	wxFileDialog fdialog( this, _T("Choose a sample"), _T("./samples"), _T(""), _T("Wave Files (*.wav) |*.wav||"), wxFD_OPEN|wxFD_CHANGE_DIR );

	wxString filename;

	if( fdialog.ShowModal() != wxID_OK )
	{
		return;
	}

	filename = fdialog.GetPath();
	_txtFilename->SetLabel( fdialog.GetPath() );

    Refresh();

	event.Skip();
}

void SampleDataDlg::OnClear( wxCommandEvent& )
{
	_txtFilename->SetValue(_(""));
	Refresh();
}
