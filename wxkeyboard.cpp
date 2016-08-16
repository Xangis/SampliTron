#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#include "wx/aboutdlg.h"
#include "sampledatadlg.h"
#include "SettingsDlg.h"
#include "wxMidiSettingsDlg.h"
#include "wxWaveVisualizer.h"
#include "wxFilterSettingsDlg.h"
#include <vector>

#include "wx/wx.h"
#include "wxkeyboard.h"

IMPLEMENT_DYNAMIC_CLASS( wxKeyboard, wxDialog )

BEGIN_EVENT_TABLE( wxKeyboard, wxDialog )
    EVT_CLOSE( wxKeyboard::OnCloseWindow )
    EVT_KEY_DOWN( wxKeyboard::OnKeyDown )
    EVT_KEY_UP( wxKeyboard::OnKeyUp )
	EVT_BUTTON( ID_PANICBUTTON, wxKeyboard::OnPanic )
	EVT_BUTTON( ID_INFOBUTTON, wxKeyboard::OnInfo )
	EVT_BUTTON( ID_HELPBUTTON, wxKeyboard::OnHelp )
	EVT_BUTTON( ID_MIDIBUTTON, wxKeyboard::OnMidiSettings )
	EVT_BUTTON( ID_SETTINGSBUTTON, wxKeyboard::OnSettings )
	EVT_BUTTON( ID_SAVEBUTTON, wxKeyboard::OnSaveConfig )
	EVT_BUTTON( ID_LOADBUTTON, wxKeyboard::OnLoadConfig )
	EVT_BUTTON( ID_ADSRBUTTON, wxKeyboard::OnAdsr )
	EVT_BUTTON( ID_FILTERBUTTON, wxKeyboard::OnFilter )
    EVT_SPIN_UP( ID_BANKSPIN, wxKeyboard::OnBankSpinUp )
    EVT_SPIN_DOWN( ID_BANKSPIN, wxKeyboard::OnBankSpinDown )
    EVT_SPIN_UP( ID_PATCHSPIN, wxKeyboard::OnPatchSpinUp )
    EVT_SPIN_DOWN( ID_PATCHSPIN, wxKeyboard::OnPatchSpinDown )
	EVT_COMMAND_SCROLL( ID_MODWHEEL, wxKeyboard::OnModWheel )
	EVT_COMMAND_SCROLL( ID_PITCHWHEEL, wxKeyboard::OnPitchWheel )
    EVT_LEFT_UP(wxKeyboard::OnMouseRelease) // Catches note off outside of an octave control.
END_EVENT_TABLE()

wxKeyboard::wxKeyboard()
{
}

wxKeyboard::wxKeyboard( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

wxKeyboard::~wxKeyboard()
{
	this->Pause();
	Sleep(10);
	this->Destroy();
	Sleep(10);
#ifdef WIN32
	if( _midiInDevice != NULL )
	{
		_midiInDevice->closePort();
	}
	if( _midiOutDevice != NULL )
	{
		_midiOutDevice->closePort();
	}
    //_masteringVoice->DestroyVoice();
    SAFE_RELEASE( _xAudio2 );
	CoUninitialize();
#endif
}

/**
* Handles dialog creation, initializes audio.
*/
bool wxKeyboard::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	_midiOutputEnabled = true;
	_bufferSize = 50;
    for( int count = 0; count < 128; count++ )
    {
        _playing[count] = false;
		_sample[count] = new SampleData;
        _sample[count]->_keyboardNote = count;
        _sample[count]->_sampleNote = count;
        _sample[count]->_volume = 127;
    }
#ifdef WIN32
	_masteringVoice = NULL;
	_xAudio2 = NULL;
	_reverbAPO = NULL;
	_volumeMeterAPO = NULL;
#endif
    _midiInDevice = new RtMidiIn();
    _midiOutDevice = new RtMidiOut();
	_settingsFile = new wxSettingsFile;
    _textColour.Set( 150, 150, 250 );
    _backgroundColour.Set( 0, 8, 40 );
	// Set up default ADSR settings.
	_adsrParameters.attackTimeMsec = 20;
	_adsrParameters.decayTimeMsec = 30;
	_adsrParameters.sustainLevelPercent = 80;
	_adsrParameters.releaseTimeMsec = 100;
	_adsrParameters.enabled = false;
	_volumeMeterL = NULL;
	_volumeMeterR = NULL;
    _bankSpin = NULL;
    _patchSpin = NULL;
	_bankText = NULL;
	_patchText = NULL;
	_panicButton = NULL;
	_infoButton = NULL;
	_helpButton = NULL;
	_saveButton = NULL;
	_loadButton = NULL;
	_midiButton = NULL;
	_adsrButton = NULL;
	_filterButton = NULL;
	_settingsButton = NULL;
	_inputChannel = 1;
	_outputChannel = 1;
	_ipatch = 1;
	_ibank = 1;
	_helpCtrl = NULL;
    _octave[0] = NULL;
    _octave[1] = NULL;
    _octave[2] = NULL;
    _octave[3] = NULL;
    _octave[4] = NULL;
    _octave[5] = NULL;
    _octave[6] = NULL;
	_numOctaves = 5;
	_midiInputDeviceNumber = 0;  // MIDI Mapper
	_midiOutputDeviceNumber = 0;  // Default Output
	EnableMidiOutput(false);
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
	_octave[0]->SetFocus();

	// Open default MIDI devices.
#ifdef WIN32
	int numDevices = midiInGetNumDevs();
	if( numDevices > 0 )
	{
		SelectMidiInputDevice(_midiInputDeviceNumber);
	}
	else
	{
		wxMessageBox(_("No MIDI input devices detected.  MIDI input is disabled."));
	}
	numDevices = midiOutGetNumDevs();
	if( numDevices > 0 )
	{
		SelectMidiOutputDevice(_midiOutputDeviceNumber);
	}
	else
	{
		wxMessageBox(_("No MIDI output devices detected.  MIDI output is disabled."));
	}
#endif

	wxFileSystem::AddHandler(new wxZipFSHandler);
	_helpCtrl = new wxHtmlHelpController(wxHF_CONTENTS);
	wxStandardPaths paths = wxStandardPaths::Get();
#ifdef WIN32
	wxFileName filename = paths.GetDataDir() + _("\\samplitron.htb");
#else
	wxFileName filename = wxString(_("./samplitron.htb"));
#endif
	if( !_helpCtrl->AddBook(filename))
	{
		wxMessageBox( _("Unable to load help file.  Please make sure that samplitron.htb is in the program directory.") );
	}
	if( _icon.LoadFile(_T("samplitron.ico"), wxBITMAP_TYPE_ICO ))
	{
		SetIcon(_icon);
	}

#ifdef WIN32
	// XAudio2 and Mastering Voice
	//
	// TODO: Enumerate audio devices instead of just using the default.
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	HRESULT hr;
    UINT32 flags = 0;
//#ifdef _DEBUG
//    flags |= XAUDIO2_DEBUG_ENGINE;
//#endif
    hr = XAudio2Create( &_xAudio2, flags );
	if( FAILED( hr ))
    {
		wxMessageBox( wxString::Format(_("Unable to create IXAudio2 interface: %d\n"), hr ));
        CoUninitialize();
		return false;
    }

	// Setting XAUDIO2_FILTER_PARAMETERS with the following values is acoustically
	// equivalent to the filter being fully bypassed.
	_filterParameters.Frequency = 1.0f;
	_filterParameters.OneOverQ = 1.0f;
	_filterParameters.Type = LowPassFilter;

	/*
	// TODO: Allow user selection of sound device.

	// Enumerating devices.
	UINT32 deviceCount;
	pXAudio2->GetDeviceCount(&deviceCount);
	XAUDIO2_DEVICE_DETAILS deviceDetails;
	int preferredDevice = 0;
	for (unsigned int i = 0; i < deviceCount; i++)
	{
		pXAudio2->GetDeviceDetails(i,&deviceDetails);
		if (deviceDetails.OutputFormat.Format.nChannels > 2)
		{
			preferredDevice = i;
			break;
		}
	}

	// In addition to the output format of the device the default role of each queried audio device can be determined by
	// checking the value returned in XAUDIO2_DEVICE_DETAILS.Role. For example, if the default communications device is
	// required, the following code can be used to find it.
	XAUDIO2_DEVICE_DETAILS deviceDetails;
	int preferredDevice = 0;

	for (unsigned int i = 0; i < deviceCount; i++)
	{
		pXAudio2->GetDeviceDetails(i,&deviceDetails);
		if (deviceDetails.Role == DefaultCommunicationsDevice)
		{
			preferredDevice = i;
			break;
		}
	}

	// If no device meets the desired criteria specifying a DeviceIndex of 0 will cause the global default device to be used.
	IXAudio2MasteringVoice* pMasterVoice = NULL;
	if ( FAILED(hr = pXAudio2->CreateMasteringVoice( &pMasterVoice, XAUDIO2_DEFAULT_CHANNELS,
								XAUDIO2_DEFAULT_SAMPLERATE, 0, preferredDevice, NULL ) ) )
		return hr;

	*/

	// Mastering Voice
	hr = _xAudio2->CreateMasteringVoice( &_masteringVoice );
	if( FAILED(hr))
	{
		wxMessageBox(wxString::Format( _("Unable to create mastering voice: %d\n"), hr ));
        SAFE_RELEASE( _xAudio2 );
        CoUninitialize();
		return false;
	}

	// Create reverb.  -- NO: IT ONLY SUPPORTS FLOAT32 DATA.
	//hr = XAudio2CreateReverb(&_reverbAPO);
	//if( FAILED(hr))
	//{
	//	wxMessageBox(wxString::Format(_("Failed to create reverb effect: %d\n"), hr));
	//}
	XAUDIO2_EFFECT_DESCRIPTOR desc[1];
	//desc[0].pEffect = _reverbAPO;
	//desc[0].OutputChannels = 2;
	//desc[0].InitialState = false;

	// Create volume meter.
	hr = XAudio2CreateVolumeMeter(&_volumeMeterAPO);
	if( FAILED(hr))
	{
		wxMessageBox(wxString::Format(_("Failed to create volume meter: %d\n"), hr));
	}
	desc[0].pEffect = _volumeMeterAPO;
	desc[0].OutputChannels = 2;
	desc[0].InitialState = true;

	XAUDIO2_EFFECT_CHAIN chain;
	chain.EffectCount = 1;
	chain.pEffectDescriptors = desc;
	hr = _masteringVoice->SetEffectChain(&chain);

	// Set up reverb parameters.
	XAUDIO2FX_REVERB_PARAMETERS params;
	_masteringVoice->SetEffectParameters(0, &params, sizeof(XAUDIO2FX_REVERB_PARAMETERS));
	if( FAILED(hr))
	{
		wxMessageBox(wxString::Format( _("Unable to set effect chain: %d\n"), hr ));
	}
	// End XAudio Setup
#endif

	// Above normal thread priority so we can monitor the sound buffer a little better.
	if( wxThread::Create(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR )
	{
		wxMessageBox( _("Unable to create sound playback thread."), _("ERROR"), wxOK );
		return false;
	}
	SetPriority(75);
	this->Run();

	return true;
}


void wxKeyboard::CreateControls()
{
    wxKeyboard* itemDialog1 = this;
    wxImage spinImage;
    spinImage.LoadFile( _("spin.bmp"), wxBITMAP_TYPE_BMP );
    wxImage sliderBk;
    sliderBk.LoadFile( _("sliderbk.bmp"), wxBITMAP_TYPE_BMP );
    wxImage sliderInd;
    sliderInd.LoadFile( _("sliderind.bmp"), wxBITMAP_TYPE_BMP );

    this->SetBackgroundColour(_backgroundColour);
    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Bank"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText4->SetForegroundColour(_textColour);
	itemStaticText4->Connect(wxID_STATIC, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
    itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    _bankText = new wxStaticText( itemDialog1, ID_BANKTEXT, _T("1"), wxDefaultPosition, wxSize( 22, -1 ), 0 );
    _bankText->SetForegroundColour(_textColour);
	_bankText->Connect(ID_BANKTEXT, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
	itemBoxSizer3->Add(_bankText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	_bankSpin = new wxBitmapSpinButton( itemDialog1, ID_BANKSPIN, wxDefaultPosition, wxSize( 16, 22 ), wxSP_ARROW_KEYS|wxSP_VERTICAL );
    _bankSpin->SetBitmap( &spinImage );
	_bankSpin->Connect(ID_BANKSPIN, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
	_bankSpin->SetToolTip( _("Change patch bank" ));
	itemBoxSizer3->Add(_bankSpin, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("Patch"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText8->SetForegroundColour(_textColour);
	itemStaticText8->Connect(wxID_STATIC, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
    itemBoxSizer3->Add(itemStaticText8, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    _patchText = new wxStaticText( itemDialog1, ID_PATCHTEXT, _T("1"), wxDefaultPosition, wxSize( 22, -1 ), 0 );
    _patchText->SetForegroundColour(_textColour);
	_patchText->Connect(ID_PATCHTEXT, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
    itemBoxSizer3->Add(_patchText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	_patchSpin = new wxBitmapSpinButton( itemDialog1, ID_PATCHSPIN, wxDefaultPosition, wxSize( 16, 22 ), wxSP_ARROW_KEYS|wxSP_VERTICAL );
    _patchSpin->SetBitmap( &spinImage );
	_patchSpin->Connect(ID_PATCHSPIN, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
	_patchSpin->SetToolTip( _("Change patch number") );
	itemBoxSizer3->Add(_patchSpin, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxBitmap panicBitmap( _("exclamation.xpm"), wxBITMAP_TYPE_XPM );
	_panicButton = new wxKeylessBitmapButton( itemDialog1, ID_PANICBUTTON, panicBitmap, wxDefaultPosition, wxSize( 26, 26 ) );
	itemBoxSizer3->Add(_panicButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3 );
	_panicButton->Connect(ID_PANICBUTTON, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_panicButton->Connect(ID_PANICBUTTON, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);
	_panicButton->Connect(ID_PANICBUTTON, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
    _panicButton->SetToolTip(_("Panic: Send all notes off MIDI message"));

	wxBitmap infoBitmap( _("info.xpm"), wxBITMAP_TYPE_XPM );
	_infoButton = new wxKeylessBitmapButton( itemDialog1, ID_INFOBUTTON, infoBitmap, wxDefaultPosition, wxSize( 26, 26 ) );
	itemBoxSizer3->Add(_infoButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3 );
	_infoButton->Connect(ID_INFOBUTTON, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_infoButton->Connect(ID_INFOBUTTON, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);
	_infoButton->Connect(ID_INFOBUTTON, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
    _infoButton->SetToolTip(_("About SampliTron"));

	wxBitmap helpBitmap( _("help.xpm"), wxBITMAP_TYPE_XPM );
	_helpButton = new wxKeylessBitmapButton( itemDialog1, ID_HELPBUTTON, helpBitmap, wxDefaultPosition, wxSize( 26, 26 ) );
	itemBoxSizer3->Add(_helpButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3 );
	_helpButton->Connect(ID_HELPBUTTON, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_helpButton->Connect(ID_HELPBUTTON, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);
	_helpButton->Connect(ID_HELPBUTTON, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
    _helpButton->SetToolTip(_("Help"));

	wxBitmap midiBitmap( _("midiport.bmp"), wxBITMAP_TYPE_BMP );
	_midiButton = new wxKeylessBitmapButton( itemDialog1, ID_MIDIBUTTON, midiBitmap, wxDefaultPosition, wxSize( 26, 26 ) );
	itemBoxSizer3->Add(_midiButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3 );
	_midiButton->Connect(ID_MIDIBUTTON, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_midiButton->Connect(ID_MIDIBUTTON, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);
	_midiButton->Connect(ID_MIDIBUTTON, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
    _midiButton->SetToolTip(_("Change MIDI settings"));

	wxBitmap settingsBitmap( _("wrench.xpm"), wxBITMAP_TYPE_XPM );
	_midiButton = new wxKeylessBitmapButton( itemDialog1, ID_SETTINGSBUTTON, settingsBitmap, wxDefaultPosition, wxSize( 26, 26 ) );
	itemBoxSizer3->Add(_midiButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3 );
	_midiButton->Connect(ID_SETTINGSBUTTON, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_midiButton->Connect(ID_SETTINGSBUTTON, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);
	_midiButton->Connect(ID_SETTINGSBUTTON, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
    _midiButton->SetToolTip(_("Change program settings"));

	// The ADSR envelope does not work properly.  The problem is that for some reason, the envelope is only applied to the
	// sample on the first play through.  Subsequent retriggers ignore the envelope settings.
	//wxBitmap adsrBitmap( "adsr.xpm", wxBITMAP_TYPE_XPM );
	//_adsrButton = new wxBitmapButton( itemDialog1, ID_ADSRBUTTON, adsrBitmap, wxDefaultPosition, wxSize( 26, 26 ) );
	//itemBoxSizer3->Add(_adsrButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3 );
	//_midiButton->Connect(ID_ADSRBUTTON, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	//_midiButton->Connect(ID_ADSRBUTTON, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);
	//_midiButton->Connect(ID_ADSRBUTTON, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
	//_midiButton->SetToolTip("Edit ADSR envelope settings");

	//wxBitmap filterBitmap( "filter.xpm", wxBITMAP_TYPE_XPM );
	//_filterButton = new wxBitmapButton( itemDialog1, ID_FILTERBUTTON, filterBitmap, wxDefaultPosition, wxSize( 26, 26 ) );
	//itemBoxSizer3->Add(_filterButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3 );
	//_filterButton->Connect(ID_FILTERBUTTON, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	//_filterButton->Connect(ID_FILTERBUTTON, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	//_filterButton->Connect(ID_FILTERBUTTON, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
 //   _filterButton->SetToolTip("Edit filter settings");

	wxBitmap saveBitmap( _("disk.bmp"), wxBITMAP_TYPE_BMP );
	_saveButton = new wxKeylessBitmapButton( itemDialog1, ID_SAVEBUTTON, saveBitmap, wxDefaultPosition, wxSize( 26, 26 ) );
	itemBoxSizer3->Add(_saveButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3 );
	_saveButton->Connect(ID_SAVEBUTTON, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_saveButton->Connect(ID_SAVEBUTTON, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);
	_saveButton->Connect(ID_SAVEBUTTON, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
#ifdef DEMOVERSION
    _saveButton->SetToolTip(_("Save Sampler Configuration to Disk (Disabled in Demo)"));
#else
    _saveButton->SetToolTip(_("Save Sampler Configuration to Disk"));
#endif

	wxBitmap loadBitmap( _("openfolder.bmp"), wxBITMAP_TYPE_BMP );
	_loadButton = new wxKeylessBitmapButton( itemDialog1, ID_LOADBUTTON, loadBitmap, wxDefaultPosition, wxSize( 26, 26 ) );
	itemBoxSizer3->Add(_loadButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3 );
	_loadButton->Connect(ID_LOADBUTTON, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_loadButton->Connect(ID_LOADBUTTON, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);
	_loadButton->Connect(ID_LOADBUTTON, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
    _loadButton->SetToolTip(_("Load Sampler Configuration from Disk"));

	wxBoxSizer* itemBoxSizer3b = new wxBoxSizer(wxVERTICAL);
	_volumeMeterL = new wxVolumeMeter( itemDialog1, ID_VOLUME_LEFT, 100, wxDefaultPosition, wxSize(100, 10));
	_volumeMeterL->Connect(ID_VOLUME_LEFT, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
	itemBoxSizer3b->Add(_volumeMeterL, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

	_volumeMeterR = new wxVolumeMeter( itemDialog1, ID_VOLUME_RIGHT, 100, wxDefaultPosition, wxSize(100, 10));
	_volumeMeterR->Connect(ID_VOLUME_RIGHT, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
	itemBoxSizer3b->Add(_volumeMeterR, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);
	itemBoxSizer3->Add(itemBoxSizer3b, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer12, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	_pitchWheel = new wxBitmapSlider( itemDialog1, ID_PITCHWHEEL, 8192, 0, 16383, wxDefaultPosition, wxSize( 30, 108 ), wxSL_VERTICAL|wxSL_INVERSE ); // 30x108 pixels
	// We don't want to do this because the mod wheel is used to tweak currently-playing notes.
	//_pitchWheel->Connect(ID_PITCHWHEEL, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
	// Bitmaps don't render right.
    _pitchWheel->SetBitmaps( &sliderBk, &sliderInd );
    itemBoxSizer12->Add( _pitchWheel, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	_pitchWheel->Connect(ID_PITCHWHEEL, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_pitchWheel->Connect(ID_PITCHWHEEL, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);

	_modWheel = new wxBitmapSlider( itemDialog1, ID_MODWHEEL, 0, 0, 16383, wxDefaultPosition, wxSize( 30, 108 ), wxSL_VERTICAL|wxSL_INVERSE );
	// We don't want to do this because the mod wheel is used to tweak currently-playing notes.
	//_modWheel->Connect(ID_MODWHEEL, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
	// Bitmaps don't render right.
	_modWheel->SetBitmaps( &sliderBk, &sliderInd );
	itemBoxSizer12->Add( _modWheel, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	_modWheel->Connect(ID_MODWHEEL, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_modWheel->Connect(ID_MODWHEEL, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);

    wxBitmap _octaveBitmap(itemDialog1->GetBitmapResource(_("octave.bmp")));
	if( !_octaveBitmap.IsOk() )
	{
		wxMessageBox(_("Could not load octave.bmp. Files broken."), _("Missing File"));
	}
	wxBitmap* keyind1sml = new wxBitmap(_("btn1sml.xpm"), wxBITMAP_TYPE_XPM );
	wxBitmap* keyind1lrg = new wxBitmap(_("btn1lrg.xpm"), wxBITMAP_TYPE_XPM );
    _octave[0] = new wxOctaveCtrl( itemDialog1, ID_KEYBOARD1, _octaveBitmap, 24, this, wxDefaultPosition, wxSize(137, 99), wxNO_BORDER );
    itemBoxSizer12->Add(_octave[0], 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
	_octave[0]->SetBitmaps(keyind1sml, keyind1lrg);
	_octave[0]->Connect(ID_KEYBOARD1, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_octave[0]->Connect(ID_KEYBOARD1, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);

    _octave[1] = new wxOctaveCtrl( itemDialog1, ID_KEYBOARD2, _octaveBitmap, 36, this, wxDefaultPosition, wxSize(137, 99), wxNO_BORDER );
    itemBoxSizer12->Add(_octave[1], 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
	_octave[1]->SetBitmaps(keyind1sml, keyind1lrg);
	_octave[1]->Connect(ID_KEYBOARD2, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_octave[1]->Connect(ID_KEYBOARD2, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);

    _octave[2] = new wxOctaveCtrl( itemDialog1, ID_KEYBOARD3, _octaveBitmap, 48, this, wxDefaultPosition, wxSize(137, 99), wxNO_BORDER );
    itemBoxSizer12->Add(_octave[2], 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
	_octave[2]->SetBitmaps(keyind1sml, keyind1lrg);
	_octave[2]->Connect(ID_KEYBOARD3, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_octave[2]->Connect(ID_KEYBOARD3, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);

    _octave[3] = new wxOctaveCtrl( itemDialog1, ID_KEYBOARD4, _octaveBitmap, 60, this, wxDefaultPosition, wxSize(137, 99), wxNO_BORDER );
    itemBoxSizer12->Add(_octave[3], 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
	_octave[3]->SetBitmaps(keyind1sml, keyind1lrg);
	_octave[3]->Connect(ID_KEYBOARD4, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_octave[3]->Connect(ID_KEYBOARD4, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);

    _octave[4] = new wxOctaveCtrl( itemDialog1, ID_KEYBOARD5, _octaveBitmap, 72, this, wxDefaultPosition, wxSize(137, 99), wxNO_BORDER );
    itemBoxSizer12->Add(_octave[4], 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
	_octave[4]->SetBitmaps(keyind1sml, keyind1lrg);
	_octave[4]->Connect(ID_KEYBOARD5, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_octave[4]->Connect(ID_KEYBOARD5, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);

    _octave[5] = new wxOctaveCtrl( itemDialog1, ID_KEYBOARD4, _octaveBitmap, 84, this, wxDefaultPosition, wxSize(137, 99), wxNO_BORDER );
    itemBoxSizer12->Add(_octave[5], 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
	_octave[5]->SetBitmaps(keyind1sml, keyind1lrg);
	_octave[5]->Connect(ID_KEYBOARD6, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_octave[5]->Connect(ID_KEYBOARD6, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);
	_octave[5]->Show(false);

    _octave[6] = new wxOctaveCtrl( itemDialog1, ID_KEYBOARD5, _octaveBitmap, 96, this, wxDefaultPosition, wxSize(137, 99), wxNO_BORDER );
    itemBoxSizer12->Add(_octave[6], 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
	_octave[6]->SetBitmaps(keyind1sml, keyind1lrg);
	_octave[6]->Connect(ID_KEYBOARD7, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_octave[6]->Connect(ID_KEYBOARD7, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);
	_octave[6]->Show(false);

    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
}

bool wxKeyboard::ShowToolTips()
{
    return true;
}

wxBitmap wxKeyboard::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
    wxUnusedVar(name);
    if (name == _T("octave.bmp"))
    {
        wxBitmap bitmap(_T("octave.bmp"), wxBITMAP_TYPE_BMP);
        return bitmap;
    }
    return wxNullBitmap;
}

/**
* Enables or disables MIDI output.
*/
void wxKeyboard::EnableMidiOutput(bool enabled)
{
	_midiOutputEnabled = enabled;
}

wxIcon wxKeyboard::GetIconResource( const wxString& name )
{
    wxUnusedVar(name);
    return wxNullIcon;
}

/**
* Displays the MIDI settings dialog, changing settings based on responses.
*/
void wxKeyboard::OnMidiSettings( wxCommandEvent& )
{
	wxMidiSettingsDlg* dlg = new wxMidiSettingsDlg(this, this);
    dlg->SetForegroundColour(wxColour( 150, 150, 250 ));
    dlg->SetBackgroundColour(wxColour( 0, 8, 40 ));
	dlg->SetMidiOutputCheckbox(_midiOutputEnabled);
	dlg->SetMidiInputDeviceIndex(_midiInputDeviceNumber );
	dlg->SetMidiOutputDeviceIndex(_midiOutputDeviceNumber );
	dlg->SetMidiInputChannel(_inputChannel);
	dlg->SetMidiOutputChannel(_outputChannel);
#ifdef WIN32
	dlg->ShowModal();
#else
    dlg->Show();
#endif
	_octave[0]->SetFocus();
	delete dlg;
}

/**
* Changes the MIDI input channel.
*/
void wxKeyboard::SelectMidiInputChannel( int number )
{
    // Set MIDI Channel
	if( number > 16 )
	{
		number = 1;
	}
	AllNotesOff(); // We may or may not need to turn off all notes that were triggered by MIDI.
	_inputChannel = number;
}

/**
* Changes the MIDI output channel, turning off all notes on the previous channel.
*/
void wxKeyboard::SelectMidiOutputChannel( int number )
{
    // Set MIDI Channel
	if( number > 16 )
	{
		number = 1;
	}
	AllNotesOff();
	_outputChannel = number;
}

/**
* Handles bank up spinner events.
*/
void wxKeyboard::OnBankSpinUp( wxSpinEvent& event )
{
    // Set Bank
    int value = atoi(_bankText->GetLabel().mb_str());
	++value;
	if( value > 128 )
	{
		value = 1;
	}
	SetBank( value );
	SetPatch( _ipatch );
	event.Skip();
}

/**
* Handles bank down spinner events.
*/
void wxKeyboard::OnBankSpinDown( wxSpinEvent& event )
{
    // Set Bank
    int value = atoi(_bankText->GetLabel().mb_str());
	--value;
	if( value < 1 )
	{
		value = 128;
	}
	SetBank( value );
	SetPatch( _ipatch );
    event.Skip();
}

/**
* Changes patch banks, sending MIDI bank change messages.
*/
void wxKeyboard::SetBank( int bank, bool receivedFromMidi )
{
    // Send MIDI message
	// Bank change 0x00 0xB0 0x00 0x[bank]  - fine
	// Bank change 0x00 0x00 0x00 0x[bank]  - coarse
	_bankText->SetLabel( wxString::Format( _("%d"), bank ));
    _ibank = bank;
	if( _midiOutputEnabled && !receivedFromMidi )
	{
#ifdef WIN32
		// Coarse bank adjust: 00 (not used), 0xXX (bank), 0x00, 0xBX (channel)
		//WORD loWord = MAKEWORD( (bank - 1), 0 );  // Position 2, then position 1.
		//WORD hiWord = MAKEWORD( (175 + _outputChannel), 0 ); // 176 = 0xB0.  Position 4, then position 3.
		//DWORD fullWord = MAKELONG( hiWord, loWord );
		//midiOutShortMsg(_midiOutDevice, fullWord );
		SendMidiMessage(0, bank-1, 0, 175 + _outputChannel);
		// Fine bank adjust: 00 (not used), 0xXX (bank), 0x20, 0xBX (channel)
		//loWord = MAKEWORD( (bank - 1), 0 );  // Position 2, then position 1.
		//hiWord = MAKEWORD( (175 + _outputChannel), 0x20 ); // 176 = 0xB0.  Position 4, then position 3.
		//fullWord = MAKELONG( hiWord, loWord );
		//midiOutShortMsg(_midiOutDevice, fullWord );
#endif
	}

}

/**
* Handles patch spinner up events.
*/
void wxKeyboard::OnPatchSpinUp( wxSpinEvent& event )
{
    // Set Patch
    int value = atoi(_patchText->GetLabel().mb_str());
	++value;
	if( value > 128 )
	{
		value = 1;
	}
    // Send MIDI message
	// Patch change: 0xC[channel] + byte for patch change.
	SetPatch( value );
    event.Skip();
}

/**
* Handles settings button clicks.
*/
void wxKeyboard::OnSettings( wxCommandEvent& event )
{
	SettingsDlg dlg(this);
	dlg.SetNumOctaves(_numOctaves);
	dlg.ShowModal();
	_numOctaves = dlg.GetNumOctaves();
	for( int i = 0; i < MAX_OCTAVES; i++ )
	{
		if( i >= _numOctaves )
			_octave[i]->Show(false);
		else
			_octave[i]->Show(true);
	}
	Fit();
	_octave[0]->SetFocus();
	event.Skip();
}

/**
* Handles patch spinner down events.
*/
void wxKeyboard::OnPatchSpinDown( wxSpinEvent& event )
{
    // Set Patch
    int value = atoi(_patchText->GetLabel().mb_str());
	--value;
	if( value < 1 )
	{
		value = 128;
	}
	SetPatch( value );
	event.Skip();
}

/**
* Handles patch changes, sends MIDI patch change message.
*/
void wxKeyboard::SetPatch( int value, bool receivedFromMidi )
{
    // Send MIDI message
	// Patch change: 0xC[channel] + byte for patch change.
	_patchText->SetLabel( wxString::Format( _("%d"), value ));
    _ipatch = value;
	if( _midiOutputEnabled && !receivedFromMidi )
	{
#ifdef WIN32
		//WORD loWord( MAKEWORD( 00, 00 ));
		//WORD hiWord( MAKEWORD( (_outputChannel + 191), (value - 1) ) );
		//DWORD fullWord = MAKELONG( hiWord, loWord );
		//midiOutShortMsg( _midiOutDevice, fullWord );
		SendMidiMessage(0, 0, value - 1, 191 + _outputChannel);
#endif
	}
}

/**
* Handles window close events.
*/
void wxKeyboard::OnCloseWindow( wxCloseEvent& event )
{
    this->Destroy();
	Sleep(12);
    event.Skip();
}

/**
* Handles key down events.
*/
void wxKeyboard::OnKeyDown( wxKeyEvent& event )
{
    int key = event.GetKeyCode();
	if( key == WXK_ALT )
	{
		ShowMappedSamples(true);
		return;
	}
    int note = GetNoteValue( key );
	if( note == 0 )
	{
		event.Skip();
		return;
	}
	// Only play if the note is not already playing -- prevents retriggers.
	if( !_playing[note] )
	{
		PlayNote( note );
	}
	event.Skip();
}

/**
* Handles key up events, turns off a playing note.
*/
void wxKeyboard::OnKeyUp( wxKeyEvent& event )
{
	int key = event.GetKeyCode();
	if( key == WXK_ALT )
	{
		ShowMappedSamples(false);
		return;
	}
	int note = GetNoteValue( key );
	if( note == 0 )
	{
		event.Skip();
		return;
	}
	StopNote( note );
    event.Skip(false);
}

void wxKeyboard::ShowMappedSamples(bool show)
{
	if( show )
	{
		bool keys[12];
		for( int i = 0; i < MAX_OCTAVES; i++ )
		{
			for( int j = 0; j < 12; j++ )
			{
				keys[j] = _sample[(i*12+j+24)]->_userSpecified;
				_octave[i]->SetIndicators(&keys[0]);
			}
		}
	}
	else
	{
		for( int i = 0; i < MAX_OCTAVES; i++ )
		{
			_octave[i]->ClearIndicators();
		}
	}
}

/**
* Gets the MIDI note number from a keyboard code.
*/
int wxKeyboard::GetNoteValue( int value )
{
	int note = 0;
    switch( value )
    {
    case WXK_F1:
        note = 24;
        break;
    case WXK_F2:
        note = 25;
        break;
    case WXK_F3:
        note = 26;
        break;
    case WXK_F4:
        note = 27;
        break;
    case WXK_F5:
        note = 28;
        break;
    case WXK_F6:
        note = 29;
        break;
    case WXK_F7:
        note = 30;
        break;
    case WXK_F8:
        note = 31;
        break;
    case WXK_F9:
        note = 32;
        break;
    case WXK_F10:
        note = 33;
        break;
    case WXK_F11:
        note = 34;
        break;
    case WXK_F12:
        note = 35;
        break;
    case '1':
        note = 36;
        break;
    case '2':
        note = 37;
        break;
    case '3':
        note = 38;
        break;
    case '4':
        note = 39;
        break;
    case '5':
        note = 40;
        break;
    case '6':
        note = 41;
        break;
    case '7':
        note = 42;
        break;
    case '8':
        note = 43;
        break;
    case '9':
        note = 44;
        break;
    case '0':
        note = 45;
        break;
    case '-':
        note = 46;
        break;
	case 43:
        note = 47;
        break;
    case 81: // q
        note = 48;
        break;
    case 87: // w
        note = 49;
        break;
    case 69: // e
        note = 50;
        break;
    case 82: // r
        note = 51;
        break;
    case 84: // t
        note = 52;
        break;
    case 89: // y
        note = 53;
        break;
    case 85: //
        note = 54;
        break;
    case 73: // i
        note = 55;
        break;
    case 79: // o
        note = 56;
        break;
    case 80: // p
        note = 57;
        break;
    case '[':
        note = 58;
        break;
    case ']':
        note = 59;
        break;
    case 65: // a
        note = 60;
        break;
    case 83: // s
        note = 61;
        break;
    case 68: // d
        note = 62;
        break;
    case 70: // f
        note = 63;
        break;
    case 71: // g
        note = 64;
        break;
    case 72: // h
        note = 65;
        break;
    case 74: // j
        note = 66;
        break;
    case 75: // k
        note = 67;
        break;
    case 76: // l
        note = 68;
        break;
    case ';':
        note = 69;
        break;
    case '\'':
        note = 70;
        break;
    case '\r':
        note = 71;
        break;
	case 90: // z
        note = 72;
        break;
    case 88: // x
        note = 73;
        break;
    case 67: // c
        note = 74;
        break;
    case 86: // v
        note = 75;
        break;
    case 66: // b
        note = 76;
        break;
    case 78: // n
        note = 77;
        break;
    case 77: // m
        note = 78;
        break;
    case ',':
        note = 79;
        break;
    case '.':
        note = 80;
        break;
    case '/':
        note = 81;
        break;
	case 306: // (right) shift
		note = 82;
		break;
	case 92: // backslash
		note = 83;
		break;
    default:
		note = 0;
        break;
    }
	return note;
}

/**
* Gets the number of notes currently playing.  Used for restricting polyphony in demo version.
*/
int wxKeyboard::GetNumNotesPlaying()
{
	int notes = 0;
	for( int i = 0; i < MAX_NOTES; i++ )
	{
		if( _playing[i] ) ++notes;
	}
	return notes;
}

/**
* Turns on a note and sends MIDI note on data.
*/
void wxKeyboard::PlayNote( int note, bool receivedFromMidi )
{
#ifdef DEMOVERSION
	if( _playing[note] == false && GetNumNotesPlaying() < 2)
#else
	if( _playing[note] == false )
#endif
	{
		// Find out which octave control we're using so we can light the note on light.
		int octavenum = GetOctaveByNote(note);
		if( octavenum >= 0 && octavenum < MAX_OCTAVES )
		{
			// Turn on one of the 12 available notes in the octave.
			_octave[octavenum]->NoteIndicatorOn((note % 12));
		}

#ifdef WIN32
		if( _midiOutputEnabled && !receivedFromMidi )
		{
			// 00 (not used), 7F (velocity), 2B (note number), 9X (note on)+channel
			char volume = _sample[note]->_volume;
			//WORD loWord = MAKEWORD( volume, 00 );
			//WORD hiWord = MAKEWORD( (143 + _outputChannel), note );
			//DWORD fullWord = MAKELONG( hiWord, loWord );
			//midiOutShortMsg(_midiOutDevice, fullWord );
			SendMidiMessage( 0, volume, note, 143 + _outputChannel );
		}

		_playing[note] = true;

		if( _sample[note]->_sourceVoice != NULL && _sample[note]->_buffer != NULL )
		{
			HRESULT hr = _sample[note]->_sourceVoice->SetVolume( ((float)_sample[note]->_volume / 127.0) );
			if( FAILED(hr))
			{
				wxMessageBox(wxString::Format(_("Unable to set volume: %d (%x) %s"), hr, hr, SampleData::GetXAudioErrorString(hr)));
				_sample[note]->_sourceVoice->DestroyVoice();
				return;
			}
			hr = _sample[note]->_sourceVoice->SubmitSourceBuffer( _sample[note]->_buffer );
			if( FAILED(hr))
			{
				wxMessageBox(wxString::Format(_("Unable to submit source buffer: %d (%x) %s"), hr, hr, SampleData::GetXAudioErrorString(hr)));
				_sample[note]->_sourceVoice->DestroyVoice();
				return;
			}
			hr = _sample[note]->_sourceVoice->Start( 0, XAUDIO2_COMMIT_NOW );
			if( FAILED( hr ))
			{
				wxMessageBox(wxString::Format(_("Unable to start source voice: %d (%x) %s"), hr, hr, SampleData::GetXAudioErrorString(hr)));
			}
		}
#endif
	}
}

/**
* Takes the note number and figures out which octave object it refers to.
*/
int wxKeyboard::GetOctaveByNote( int note )
{
	// Minimum octave and below is translated to minimum octave.
	if( note < 36 )
	{
		return 0;
	}
	// Maximum octave and above is translated to maximum octave.
	else if( note >= 96 )
	{
		return 6;
	}
	else
	{
		return (note - 24) / 12;
	}
}

/**
* Turns off a note, sends MIDI note off data.
*/
void wxKeyboard::StopNote( int note, bool receivedFromMidi )
{
	int octavenum = GetOctaveByNote(note);
	if( octavenum >= 0 && octavenum < MAX_OCTAVES )
	{
		// Turn off one of the 12 available notes in the octave.
		_octave[octavenum]->NoteIndicatorOff((note % 12 ));
	}

#ifdef WIN32
	if( _sample[note]->_sourceVoice != NULL )
	{
		_sample[note]->_sourceVoice->Stop();
		_sample[note]->_sourceVoice->FlushSourceBuffers();
	}

	if( _midiOutputEnabled && !receivedFromMidi )
	{
		// We send the note off regardless of whether it's actually on or not - we may 
		// have had a mistriggered event.
		// 00 (not used), 00 (velocity), 2B (note number), 80 (note off)+channel
		//WORD loWord = MAKEWORD( 0, 0 );
		//WORD hiWord = MAKEWORD( (127 + _outputChannel), note );
		//DWORD fullWord = MAKELONG( hiWord, loWord );
		//midiOutShortMsg(_midiOutDevice, fullWord );
		SendMidiMessage( 0, 0, note, 127 + _outputChannel );
	}
#endif
	_playing[note] = false;
}

/**
* Processes mouse right clicks, displays sample editing dialog.
*/
void wxKeyboard::OnRightClick( int note )
{
    // Show sample edit dialog with settings for note.
    SampleDataDlg sampleDialog(this);
    sampleDialog.SetSample( _sample[note] );
    sampleDialog.ShowModal();
	SampleData* data = sampleDialog.GetSample();
	if( data->_filename.Length() == 0 )
	{
		ClearSampleData(note);
		_sample[note] = data;
	}
	else
	{
		ClearSampleData(note);
		_sample[note] = data;
		_sample[note]->_userSpecified = true;
		if( !LoadSample(note) )
		{
			wxMessageBox(wxString::Format(_("Failed to load sample %s."), _sample[note]->_filename.c_str()), _("Sample Load Error"));
		}
	}
	RefreshSampleData();
}

// Refreshes sample data across the scale of the keyboard when an individual sample is added or removed.
//
// This should resample all necessary samples when "conserve CPU" is set, and should not when "Conserve Memory"
// is set.
void wxKeyboard::RefreshSampleData()
{
	std::vector<int> specifiedSamples;
	for( int i = 0; i < MAX_NOTES; i++ )
	{
		if( _sample[i]->_userSpecified == true )
			specifiedSamples.push_back(i);
	}
	//wxMessageBox(wxString::Format("%d samples have been specified.", specifiedSamples.size()));
	if( specifiedSamples.size() == 0 )
	{
		// Go through and clear everything.
		for( int i = MIN_ALLOWABLE_SAMPLE; i <= MAX_ALLOWABLE_SAMPLE; i++ )
		{
			_sample[i]->_filename = _("");
			_sample[i]->_keyboardNote = i;
			_sample[i]->_looping = false;
			ClearSampleData(i); // Nuke sample data, number of samples, user specified, and wave file.
			_sample[i]->_sampleNote = i;
			_sample[i]->_volume = 127;
		}
	}
	else if( specifiedSamples.size() == 1 )
	{
		int specifiedOne = specifiedSamples[0];
		// Go through and set everything to the specified sample.
		// We may want a progres bar for this.
		CopySampleData(MIN_ALLOWABLE_SAMPLE, MAX_ALLOWABLE_SAMPLE, specifiedOne );
	}
	else
	{
		// Fill in the everything up to the first user-specified sample.
		int previous = specifiedSamples[0];
		if( previous > MIN_ALLOWABLE_SAMPLE )
		{
			CopySampleData(MIN_ALLOWABLE_SAMPLE, previous-1, previous);
		}
		int current = specifiedSamples[0];
		int diff = 0;
		// Fill in data for the second through the next-to-last user-specified sample.
		for( unsigned int iterator = 1; iterator < specifiedSamples.size() - 1; iterator++ )
		{
			previous = specifiedSamples[iterator - 1];
			current = specifiedSamples[iterator];
			diff = current - previous;
			// No work to do if two samples in a row are specified.
			if ( diff == 1 )
			{
				continue;
			}
			// Only one between, we take the previous sample.
			else if( diff == 2 )
			{
				CopySampleData(previous+1, previous+1, previous);
				continue;
			}
			// Split the difference and fill the first half with the previous and the
			// second half with the current.
			int firstStop = previous + (diff / 2);
			int secondStart = current - (diff / 2);
			CopySampleData( previous + 1, firstStop, previous );
			CopySampleData( secondStart + 1, current - 1, current );
		}
		// Fill in data up to the last sample.
		int newCurrent = current + ((specifiedSamples[specifiedSamples.size() - 1] - current) / 2);
		if( current < MAX_ALLOWABLE_SAMPLE && (specifiedSamples[specifiedSamples.size() - 1] > current+1) )
		{
			CopySampleData( current + 1, newCurrent, current );
		}
		// Fill in data for the last sample.
		if( current < MAX_ALLOWABLE_SAMPLE )
		{
			CopySampleData( newCurrent + 1, MAX_ALLOWABLE_SAMPLE, specifiedSamples[(specifiedSamples.size() - 1)] );
		}
	}
}

/**
* Copies sample data from the source to a range of samples.  Used when extrapolating one or more
* user-specified sample across non-specified keys.
*/
void wxKeyboard::CopySampleData(int start, int end, int source)
{
	if( end < start )
		return;
	for( int i = start; i <= end; i++ )
	{
		// Don't overwrite the template if it's within our data range.
		if( i == source )
		{
			continue;
		}
		// If the filename matches and our target sample has data we'll assume that
		// it has already been mapped and keep going.
#ifdef WIN32
		if( _sample[i]->_filename == _sample[source]->_filename &&
			_sample[i]->_sourceVoice != NULL )
		{
			continue;
		}
#endif
		// Set sample[currentSample]'s data same as that from first.
		_sample[i]->_filename = _sample[source]->_filename;
		_sample[i]->_keyboardNote = i;
		_sample[i]->_looping = _sample[source]->_looping;
		_sample[i]->_sampleNote = _sample[source]->_sampleNote;
		_sample[i]->_userSpecified = false;
		_sample[i]->_volume = _sample[source]->_volume;
		LoadSample(i);
	}
}

/** 
* Clears the sample data when a sample has had its filename cleared or when we
* are replacing it with another sample.
*/
void wxKeyboard::ClearSampleData(int samplNum)
{
	if( _sample[samplNum]->_waveFile != NULL )
	{
		_sample[samplNum]->_waveFile = NULL;
		delete _sample[samplNum]->_waveFile;
	}
	_sample[samplNum]->DestroySource();
	_sample[samplNum]->_userSpecified = false;
}

/**
* Loads a sample number from the filename specified in the sample.
* Deletes existing data and resamples as necessary based on the existing sample
* data.
*/
bool wxKeyboard::LoadSample(int sampleNum)
{
#ifdef WIN32
	if( _sample[sampleNum]->_waveFile != NULL || _sample[sampleNum]->_sourceVoice != NULL )
	{
		ClearSampleData(sampleNum);
	}
#endif

	_sample[sampleNum]->_waveFile = WaveFile::Load( _sample[sampleNum]->_filename.mb_str(), true );
	if( !_sample[sampleNum]->_waveFile)
	{
		return false;
	}

	if(_sample[sampleNum]-> _sampleNote != _sample[sampleNum]->_keyboardNote )
	{
		// 12th root frequency scaling (per-semitone note scaling).
		// This means that a note of 36 played at keyboard position 48 will need to scale +12.
		double noteDifference = _sample[sampleNum]->_keyboardNote - _sample[sampleNum]->_sampleNote;
		double ratio = pow(1.0594630943592952, noteDifference);
#ifdef WIN32
		_sample[sampleNum]->CreateSource(_xAudio2);
		HRESULT hr = _sample[sampleNum]->_sourceVoice->SetFilterParameters(&_filterParameters, XAUDIO2_COMMIT_NOW);
		_xAudio2->CommitChanges(XAUDIO2_COMMIT_ALL);
		if( FAILED( hr ))
		{
			wxMessageBox( wxString::Format(_("Unable to create set source voice filter parameters: %d\n"), hr ));
		}
		_sample[sampleNum]->_sourceVoice->SetFrequencyRatio(ratio);
#endif
	}
	else
	{
#ifdef WIN32
		_sample[sampleNum]->CreateSource(_xAudio2);
		HRESULT hr = _sample[sampleNum]->_sourceVoice->SetFilterParameters(&_filterParameters, XAUDIO2_COMMIT_NOW);
		_xAudio2->CommitChanges(XAUDIO2_COMMIT_ALL);
		if( FAILED( hr ))
		{
			wxMessageBox( wxString::Format(_("Unable to create set source voice filter parameters: %d\n"), hr ));
		}
#endif
	}

	return true;
	// Show resampled data.
	//wxWaveVisualizer viz2(this, _sample[note]->_sampleData, _sample[note]->_sampleDataLength, _sample[note]->_waveFile->GetBytesPerSample(), _sample[note]->_waveFile->GetNumChannels());
	//viz2.ShowModal();
}

#ifdef WIN32
/**
* Callback for MIDI message data.
*/
//void CALLBACK MidiMessageHandler(HMIDIIN hMidiIn, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
void MidiMessageHandler( double deltatime, std::vector< unsigned char > *message, void *userData )
{
	wxKeyboard* key = (wxKeyboard*)userData;

    unsigned char a = 0;
    unsigned char b = 0;
    unsigned char c = 0;
    unsigned char d = 0;
    if( message->size() > 0 )
    {
        d = (*message)[0];
    }
    if( message->size() > 1 )
    {
        c = (*message)[1];
    }
    if( message->size() > 2 )
    {
        b = (*message)[2];
    }
    if( message->size() > 3 )
    {
        a = (*message)[3];
    }
	if( message->size() > 4 )
	{
		printf( "MIDI message size too large, cannot process." );
		return;
	}
	key->ProcessMidiMessage(a, b, c, d);
}

/**
* Handles incoming MIDI message data.
*/
//void wxKeyboard::ProcessMidiMessage(DWORD dwParam1, DWORD dwParam2)
void wxKeyboard::ProcessMidiMessage(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4)

{
	// MIDI timimg clock pulse.  Doesn't mean anything to us.
	if( byte4 == 0xF8 )
	{
		return;
	}

	// Active sensing message.  Doesn't mean anything to us.
	if( byte4 == 0xFE )
	{
		return;
	}

	if( byte4 == (0x90 + _inputChannel - 1) )
	{
		if( byte2 > 0 )
		{
			_sample[byte3]->_volume = byte2;
			PlayNote(byte3);
		}
		else
		{
			StopNote(byte3);
		}
		return;
	}
	// Note off (some keyboards send this instead of note on 0 velocity.
	if( byte4 == (0x80 + _inputChannel - 1)  )
	{
		StopNote( byte3 );
		return;
	}
	// Program change.
	else if( byte4 == (0xC0 + _inputChannel - 1)  )
	{
		SetPatch( byte3 );
		return;
	}
	else if( byte4 == (0xB0 + _inputChannel - 1)  )
	{
		// All notes off.
		if( byte3 == 0x7B && byte2 == 0x00 )
		{
			AllNotesOff();
		}
	}
	//MIDI Message Data Bytes Values
	//Note Off 8n kk vv kk = key number (0-127)
	//vv = velocity (0-127)
	//Note On
	//9n kk vv kk = key number (0-127)
	//vv = velocity (1-127, 0 = note off)
	//Polyphonic After Touch An kk tt kk = key number (0-127)
	//tt = after touch (0-127)
	//Control Change
	//Bn cc xx cc = controller (0-121)
	//xx = value (0-127)
	//Channel Mode Local Control Bn 7A xx xx = 0 (off), 127 (on)
	//All Notes Off Bn 7B 00
	//Omni Mode Off Bn 7C 00
	//Omni Mode On Bn 7D 00
	//Mono Mode On Bn 7E cc cc = number of channels
	//Poly Mode On Bn 7F 00
	//Program Change Cn pp pp = program (0-127)
	//Channel After Touch Dn tt tt = after touch (0-127)
	//Pitch Wheel Change En ll hh ll = low 7 bits (0-127)
	//hh = high 7 bits (0-127)
}
#endif

/**
* Processes MIDI input device selection changes.
*/
void wxKeyboard::SelectMidiInputDevice(int number)
{
	_midiInputDeviceNumber = number;
#ifdef WIN32
	// Close the old device
	try
	{
		// Close existing MIDI device.
		if( _midiInDevice != NULL )
		{
			_midiInDevice->closePort();
		}
		// Open the new MIDI Device
		_midiInDevice->openPort(number);
		_midiInDevice->setCallback(MidiMessageHandler);
	}
	catch( RtMidiError &error )
	{
		wxMessageBox( wxString::FromAscii(error.what()), _("Error opening MIDI input"));
		//wxMessageBox(wxString::Format(_("There was an error starting the MIDI input device %d: %d (%s)"), number, result, GetMidiError(result)), "MIDI Error");
	}
#endif
}

/**
* Processes MIDI output device selection changes.
*/
void wxKeyboard::SelectMidiOutputDevice(int number)
{
	_midiOutputDeviceNumber = number;
#ifdef WIN32
	try
	{
		// Close the old device
		if( _midiOutDevice != NULL )
		{
			_midiOutDevice->closePort();
		}
		// Open the MIDI Device
		_midiOutDevice->openPort(number);
	}
	catch( RtMidiError &error )
	{
		wxMessageBox(wxString::FromAscii(error.what()), _("Error opening MIDI output"));
		//wxMessageBox(wxString::Format(_("There was an error opening the MIDI output device %d: %d (%s)"), number, result, GetMidiError(result)), "MIDI Error");
	}
#endif
}

/**
* Handles pitch wheel messages, sends MIDI controller data.
*/
void wxKeyboard::OnPitchWheel( wxScrollEvent& event )
{
	int value;
	if( event.GetEventType() == wxEVT_SCROLL_THUMBRELEASE )
	{
		value = 8192;
		_pitchWheel->SetValue(8192);
		Refresh();
	}
	else
	{
		value = _pitchWheel->GetValue();
	}
	// We may just want to use the fine value for this - coarse might be what hoses us.
	int coarse = value / 128;
	int fine = value % 128;
	if( _midiOutputEnabled )
	{
		// 00 (not used), XX (coarse), XX (fine), 0xEx (message + channel)
#ifdef WIN32
		//WORD loWord = MAKEWORD( coarse, 0 );
		//WORD hiWord = MAKEWORD( (223 + _outputChannel), fine );
		//DWORD fullWord = MAKELONG( hiWord, loWord );
		//midiOutShortMsg(_midiOutDevice, fullWord );
		SendMidiMessage( 0, coarse, fine, 223 + _outputChannel );
#endif
	}
	event.Skip();
}

/**
* Handles mod wheel messages, sends MIDI controller data.
*/
void wxKeyboard::OnModWheel( wxScrollEvent& event )
{
	int value = _modWheel->GetValue();
	int coarse = value / 128;
	int fine = value % 128;
	if( _midiOutputEnabled )
	{
		// Modwheel message is two parts - coarse and fine.
		// 00 (not used), 0xXX (value:coarse), 0x01 (controller), 0xBX (175 + channel)
#ifdef WIN32
		//WORD loWord = MAKEWORD( coarse, 0 );
		//WORD hiWord = MAKEWORD( (175 + _outputChannel), 1 );
		//DWORD fullWord = MAKELONG( hiWord, loWord );
		//midiOutShortMsg(_midiOutDevice, fullWord );
		SendMidiMessage( 0, coarse, 1, 175 + _outputChannel );
		// 00 (not used), 0xXX (value:fine), 0x21 (controller), 0xBX (175 + channel)
		//loWord = MAKEWORD( fine, 0 );
		//hiWord = MAKEWORD( (175 + _outputChannel), 0x21 );
		//fullWord = MAKELONG( hiWord, loWord );
		//midiOutShortMsg(_midiOutDevice, fullWord );
		SendMidiMessage( 0, fine, 0x21, 175 + _outputChannel );
#endif
	}
	event.Skip();
}

/**
* Handles panic button, turns everything off.
*/
void wxKeyboard::OnPanic( wxCommandEvent& event )
{
	AllNotesOff();
	AllControllersOff();
	AllSoundOff();
	_octave[0]->SetFocus();
	event.Skip();
}

/**
* Sends MIDI all sounds off message.
*/
void wxKeyboard::AllSoundOff( void )
{
	if( _midiOutputEnabled )
	{
		// 00 (not used), 0x00, 120, 0xBX (message + channel)
#ifdef WIN32
		//WORD loWord = MAKEWORD( 0, 0 );
		//WORD hiWord = MAKEWORD( (175 + _outputChannel), 120 );
		//DWORD fullWord = MAKELONG( hiWord, loWord );
		//midiOutShortMsg(_midiOutDevice, fullWord );
		SendMidiMessage( 0, 0, 120, 175 + _outputChannel );
#endif
	}
}

/**
* Handles release of the left mouse button.
*/
void wxKeyboard::OnMouseRelease( wxMouseEvent& event )
{
	AllNotesOff();
	event.Skip();
}

/**
* Turns off all notes and indicators.
*/
void wxKeyboard::AllNotesOff( bool receivedFromMidi )
{
	for( int i = 0; i < MAX_NOTES; i++ )
	{
		if( _playing[i] )
		{
			StopNote(i);
		}
		_playing[i] = false;
	}
	for(int i = 0; i < MAX_OCTAVES; i++ )
	{
		_octave[i]->TurnOffAll();
	}
	if( _midiOutputEnabled && !receivedFromMidi )
	{
		// 00 (not used), 0x00, 123, 0xBX (message + channel)
#ifdef WIN32
		//WORD loWord = MAKEWORD( 0, 0 );
		//WORD hiWord = MAKEWORD( (175 + _outputChannel), 123 );
		//DWORD fullWord = MAKELONG( hiWord, loWord );
		//midiOutShortMsg(_midiOutDevice, fullWord );
		SendMidiMessage( 0, 0, 123, 175 + _outputChannel );
#endif
	}
	// Turn off any red dots.
	Refresh();
}

/**
* Turns off all MIDI controllers.
*/
void wxKeyboard::AllControllersOff( void )
{
	if( _midiOutputEnabled )
	{
		// 00 (not used), 0x00, 121, 0xBX (message + channel)
#ifdef WIN32
		//WORD loWord = MAKEWORD( 0, 0 );
		//WORD hiWord = MAKEWORD( (175 + _outputChannel), 121 );
		//DWORD fullWord = MAKELONG( hiWord, loWord );
		//midiOutShortMsg(_midiOutDevice, fullWord );
		SendMidiMessage( 0, 0, 121, 175 + _outputChannel );
#endif
	}
	// Reset modwheel position
	_modWheel->SetValue(0);
	_pitchWheel->SetValue(8192);
	Refresh();
}

/**
* Loads a sampler config from disk and sets up the key map.
*/
void wxKeyboard::OnLoadConfig( wxCommandEvent& event )
{
	wxStandardPaths paths = wxStandardPaths::Get();
	wxString path = paths.GetDataDir() + _("\\patches");
	wxFileDialog fdialog( this, _T("Load A Config"), path, _T(""), _T("Sampler Configurations (*.samp) |*.samp||"), wxFD_OPEN );

	wxString filename;

	if( fdialog.ShowModal() != wxID_OK )
	{
		_octave[0]->SetFocus();
		return;
	}

	wxString value;
	wxSettingsFile file;
	wxString fname = fdialog.GetPath().MakeLower();
	file.Load(fname);
	wxString failures = wxEmptyString;
	for( int i = 0; i < MAX_NOTES; i++ )
	{
		// Clear existing sample and re-create so we don't get any data remnants or "merged configs".
		delete _sample[i];
		_sample[i] = new SampleData;
		value = file.GetValue(wxString::Format(_("Key%dSampleNote"), i));
		if( value.Length() > 0 )
		{
			_sample[i]->_sampleNote = atoi(value.mb_str());
		}
		value = file.GetValue(wxString::Format(_("Key%dKeyboardNote"), i));
		if( value.Length() > 0 )
		{
			_sample[i]->_keyboardNote = atoi(value.mb_str());
		}
		value = file.GetValue(wxString::Format(_("Key%dLooping"), i));
		if( value.Length() > 0 )
		{
			_sample[i]->_looping = (bool)atoi(value.mb_str());
		}
		value = file.GetValue(wxString::Format(_("Key%dSampleFile"), i));
		if( value.Length() > 0 )
		{
			_sample[i]->_filename = value;
			if ( !LoadSample(i) )
			{
				failures = failures.Append(wxString::Format(_("Unable to load sample %s.\n"), _sample[i]->_filename.c_str()));
			}
		}
		value = file.GetValue(wxString::Format(_("Key%dVolume"), i));
		if( value.Length() > 0 )
		{
			_sample[i]->_volume = atoi(value.mb_str());
		}
		value = file.GetValue(wxString::Format(_("Key%dSpecified"), i));
		if( value.Length() > 0 )
		{
			_sample[i]->_userSpecified = (bool)atoi(value.mb_str());
		}
	}
	if( failures.Length() > 0 )
	{
		wxMessageBox( failures, _("Sample Load Error") );
	}
	RefreshSampleData();
	_octave[0]->SetFocus();
	event.Skip(false);
}

/**
* Saves a sampler config to disk.
*/
void wxKeyboard::OnSaveConfig( wxCommandEvent& event )
{
#ifdef DEMOVERSION
#else
	wxStandardPaths paths = wxStandardPaths::Get();
	wxString path = paths.GetDataDir() + _("\\patches");
	wxFileDialog fdialog( this, _T("Save Config As"), path, _T(""), _T("Sampler Configurations (*.samp) |*.samp||"), wxFD_SAVE );

	wxString filename;

	if( fdialog.ShowModal() != wxID_OK )
	{
		_octave[0]->SetFocus();
		return;
	}

	wxSettingsFile file;
	for( int i = 0; i < MAX_NOTES; i++ )
	{
		file.SetValue(wxString::Format(_("Key%dSampleNote"), i), wxString::Format(_("%d"), _sample[i]->_sampleNote ));
		file.SetValue(wxString::Format(_("Key%dKeyboardNote"), i), wxString::Format(_("%d"), _sample[i]->_keyboardNote ));
		file.SetValue(wxString::Format(_("Key%dLooping"), i),  wxString::Format(_("%d"), _sample[i]->_looping ));
		file.SetValue(wxString::Format(_("Key%dVolume"), i), wxString::Format(_("%d"), _sample[i]->_volume ));
		file.SetValue(wxString::Format(_("Key%dSpecified"), i),  wxString::Format(_("%d"), _sample[i]->_userSpecified ));
		if( _sample[i]->_userSpecified )
		{
			wxString exePath = paths.GetDataDir().MakeLower();
			wxString fileLocation = _sample[i]->_filename.MakeLower();
			if( fileLocation.StartsWith(exePath))
			{
				fileLocation = _(".") + fileLocation.SubString(exePath.Length(), (fileLocation.Length() - 1));
			}
			file.SetValue(wxString::Format(_("Key%dSampleFile"), i), fileLocation );
		}
	}
	wxString name = fdialog.GetPath();
	file.Save(name);
#endif
	_octave[0]->SetFocus();
	event.Skip(false);
}

/**
* Shows help file.
*/
void wxKeyboard::OnHelp( wxCommandEvent& event )
{
	// Show help file.
	_helpCtrl->DisplayContents();
	_octave[0]->SetFocus();
	event.Skip();
}

/**
* Shows the dialog for editing ADSR envelope.
*/
void wxKeyboard::OnAdsr( wxCommandEvent& event )
{
	wxADSRDlg* dlg = new wxADSRDlg(this);
	dlg->SetForegroundColour(wxColour( 150, 150, 250 ));
	dlg->SetBackgroundColour(wxColour( 0, 8, 40 ));
	dlg->SetAdsrParameters(_adsrParameters);
#ifdef WIN32
	dlg->ShowModal();
#else
    dlg->Show();
#endif
	_adsrParameters = dlg->GetAdsrParameters();
}

/**
* Shows the dialog for editing filter settings.
*/
void wxKeyboard::OnFilter( wxCommandEvent& event )
{
	wxFilterSettingsDlg* dlg = new wxFilterSettingsDlg(this);
    dlg->SetForegroundColour(wxColour( 150, 150, 250 ));
    dlg->SetBackgroundColour(wxColour( 0, 8, 40 ));
#ifdef WIN32
	dlg->SetFilterParameters(_filterParameters);
	// Set enable state based on our settings.
	if( _filterParameters.Frequency == 1.0f && _filterParameters.OneOverQ == 1.0f && _filterParameters.Type == LowPassFilter )
	{
		dlg->SetEnabled(false);
	}
	else
	{
		dlg->SetEnabled(true);
	}
	dlg->ShowModal();
	// Enable or disable filter based on settings.
	if( dlg->GetEnabled() == false )
	{
		// Set filter parameters to effectively bypassed.
		_filterParameters.Frequency = 1.0f;
		_filterParameters.OneOverQ = 1.0f;
		_filterParameters.Type = LowPassFilter;
	}
	else
	{
		_filterParameters = dlg->GetFilterParameters();
	}
	// Change voice settings on ALL source voices.
	for( int i = 0; i < MAX_NOTES; i++ )
	{
		if( _sample[i]->_sourceVoice != NULL )
		{
			HRESULT hr = _sample[i]->_sourceVoice->SetFilterParameters(&_filterParameters, XAUDIO2_COMMIT_NOW );
//			XAUDIO2_FILTER_PARAMETERS params;
//			_sample[i]->_sourceVoice->GetFilterParameters(&params);
			_xAudio2->CommitChanges(XAUDIO2_COMMIT_ALL);
//			_sample[i]->_sourceVoice->GetFilterParameters(&params);
			if( FAILED( hr ))
			{
				wxMessageBox( wxString::Format(_("Unable to create set source voice filter parameters: %d\n"), hr ));
			}
		}
	}
#endif
	delete dlg;
	event.Skip();
}

/**
* Shows about box.
*/
void wxKeyboard::OnInfo( wxCommandEvent& event )
{
	// Show about box.
    wxAboutDialogInfo info;
    info.SetVersion(_("1.1"));
    info.SetCopyright(_("Copyright (c) 2006-2016 Zeta Centauri"));
	info.AddDeveloper(_("Jason Champion"));
	info.SetIcon(_icon);
#ifdef DEMOVERSION
	info.SetLicense(_("The demo version of SampliTron may be distributed freely.\n\nThe included samples are from FreeWaveSamples.com and\nare copyright (c) 2006-2013 Zeta Centauri.  They may\nbe distributed freely and be used royalty-free for any purpose.\n\nThe software is provided 'as is', without warranty of any kind,\nexpress or implied, including but not limited to the warranties\nof merchantability, fitness for a particular purpose and\nnoninfringement.  In no event shall the authors or copyright\nholders be liable for any claim, damages or other liability,\nwhether in an action of contract, tort or otherwise, arising\nfrom, out of or in connection with the software or the use\nor other dealings in the software."));
    info.SetName(_("SampliTron Demo"));
#else
	info.SetLicense(_("SampliTron is copyrighted software and may not be distributed without a license.\n\nThe included samples are from FreeWaveSamples.com and\nare copyright (c) 2006-2016 Zeta Centauri, Inc.  They may\nbe distributed freely and be used royalty-free for any purpose.\n\nThe software is provided 'as is', without warranty of any kind,\nexpress or implied, including but not limited to the warranties\nof merchantability, fitness for a particular purpose and\nnoninfringement.  In no event shall the authors or copyright\nholders be liable for any claim, damages or other liability,\nwhether in an action of contract, tort or otherwise, arising\nfrom, out of or in connection with the software or the use\nor other dealings in the software."));
    info.SetName(_("SampliTron"));
#endif
	info.SetWebSite(_("http://zetacentauri.com"));
	info.SetDescription(_("SampliTron uses the wxWidgets and libsndfile libraries."));

    wxAboutBox(info);
	_octave[0]->SetFocus();
    event.Skip();
}

/**
* App background thread -- monitors buffers, etc.
*/
void * wxKeyboard::Entry()
{
#ifdef WIN32
	XAUDIO2FX_VOLUMEMETER_LEVELS levels;
	levels.pPeakLevels = new float[2];
	levels.pRMSLevels = new float[2];
	levels.ChannelCount = 2;
#endif
	while( !TestDestroy())
	{
		// Update volume meter.
#ifdef WIN32
		_masteringVoice->GetEffectParameters(0, &levels, sizeof(XAUDIO2FX_VOLUMEMETER_LEVELS));
		_volumeMeterL->SetFloatValue(levels.pPeakLevels[0]);
		_volumeMeterR->SetFloatValue(levels.pPeakLevels[1]);
#endif
		// Process envelope.
		// The ADSR envelope does not work properly.  The problem is that for some reason, the envelope is only applied to the
		// sample on the first play through.  Subsequent retriggers ignore the envelope settings.
		//if( _adsrParameters.enabled )
		//{
		//	for( int i = 0; i < MAX_NOTES; i++ )
		//	{
		//		if( _sample[i]->_sourceVoice != NULL )
		//		{
		//			XAUDIO2_VOICE_STATE voiceState;
		//			_sample[i]->_sourceVoice->GetState(&voiceState);
		//			//voiceState->BuffersQueued;
		//			//voiceState->pCurrentBufferContext;
		//			if( voiceState.SamplesPlayed < _sample[i]->_waveFile->GetNumSamples() )
		//			{
		//				//XAUDIO2_VOICE_DETAILS* voiceDetails;
		//				//_sample[i]->_sourceVoice->GetVoiceDetails(voiceDetails);
		//				//voiceDetails->CreationFlags;
		//				//voiceDetails->InputChannels;
		//				//voiceDetails->InputSampleRate;
		//				HRESULT hr = _sample[i]->_sourceVoice->SetVolume( GetEnvelopeVolume(i, voiceState.SamplesPlayed) * ((float)_sample[i]->_volume / 127.0) );
		//				if( FAILED(hr))
		//				{
		//					wxMessageBox(wxString::Format( _("Unable to set volume level on voice #%d: %d\n"), i, hr ));
		//				}
		//			}
		//		}
		//	}
		//}
		Sleep(5);
	}
#ifdef WIN32
	delete[] levels.pPeakLevels;
	delete[] levels.pRMSLevels;
#endif
	return NULL;
}

float wxKeyboard::GetEnvelopeVolume(int note, int playbackSamplePosition)
{
	if( playbackSamplePosition == 0 )
		return 0.0f;

	int millisecond = ((float)playbackSamplePosition * 1000.0f / (float)(_sample[note]->_waveFile->GetSampleRate()));
	// Attack.
	if( millisecond < _adsrParameters.attackTimeMsec )
	{
		float ramp = (float)millisecond / (float)(_adsrParameters.attackTimeMsec);
		return ramp;
	}
	// Decay
	else if( millisecond < (_adsrParameters.attackTimeMsec + _adsrParameters.decayTimeMsec) )
	{
		float ramp = (float)(millisecond - _adsrParameters.attackTimeMsec) / (float)(_adsrParameters.decayTimeMsec);
		// Value we're returning is the sustain level plus a fraction of the maximum based on
		// hor far we are through the decay.
		float value = ((float)(_adsrParameters.sustainLevelPercent) + ((100.0f - (float)(_adsrParameters.sustainLevelPercent)) * (1.0 - ramp))) / 100.0f;
		return value;
	}
	// Sustain.
	else
	{
		return (float)(_adsrParameters.sustainLevelPercent) / 100.0f;
	}
	// TODO: Release.
	return 1.0f;
}

void wxKeyboard::SendMidiMessage(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4, bool shortmsg)
{
    std::vector<unsigned char> msg;
    msg.push_back(byte4);
    msg.push_back(byte3);
    if(!shortmsg)
    {
      msg.push_back(byte2);
      if( byte1 != 0 )
      {
        msg.push_back(byte1);
      }
#ifndef VST
      _midiOutDevice->sendMessage(&msg);
#endif
    }
}