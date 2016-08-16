#ifndef _WXKEYBOARD_H_
#define _WXKEYBOARD_H_

#define MAX_OCTAVES 7
#define MAX_NOTES 128
// We use this to restrict our note range.
#define MIN_ALLOWABLE_SAMPLE 24
#define MAX_ALLOWABLE_SAMPLE 107

/*!
 * Includes
 */
#include "wx/spinctrl.h"
#include "wx/filesys.h"
#include "wx/fs_zip.h"
#include "wx/html/helpctrl.h"
#include "wx/stdpaths.h"
#include "wxBitmapSpinButton.h"
#include "wxBitmapSlider.h"
#include "wxOctaveCtrl.h"
#include "OctaveCallback.h"
#include "wxSettingsFile.h"
#include "MidiSettingsInterface.h"
#include "wxVolumeMeter.h"
#include "wxADSRDlg.h"
#include "wxKeylessBitmapButton.h"
#include "sampledata.h"
#include "sampledatadlg.h"
#ifdef WIN32
#include <XAudio2.h>
#include <xaudio2fx.h>
#define INITGUID
#endif

/*!
 * Forward declarations
 */
class wxSpinCtrl;

/*!
 * Control identifiers
 */
#define ID_KEYBOARD_DLG 10000
#define SYMBOL_WXKEYBOARD_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxWANTS_CHARS|wxMINIMIZE_BOX
#define SYMBOL_WXKEYBOARD_TITLE _("SampliTron")
#define SYMBOL_WXKEYBOARD_IDNAME ID_KEYBOARD_DLG
#define SYMBOL_WXKEYBOARD_SIZE wxSize(400, 300)
#define SYMBOL_WXKEYBOARD_POSITION wxDefaultPosition
#define ID_BANKSPIN 10001
#define ID_PATCHSPIN 10003
#define ID_KEYBOARD1 10006
#define ID_KEYBOARD2 10007
#define ID_KEYBOARD3 10008
#define ID_KEYBOARD4 10009
#define ID_KEYBOARD5 10010
#define ID_KEYBOARD6 10011
#define ID_KEYBOARD7 10012
#define ID_PATCHTEXT 10013
#define ID_CHANNELTEXT 10014
#define ID_PITCHWHEEL 10015
#define ID_MODWHEEL 10016
#define ID_PANICBUTTON 10017
#define ID_INFOBUTTON 10018
#define ID_HELPBUTTON 10019
#define ID_SAVEBUTTON 10021
#define ID_LOADBUTTON 10022
#define ID_MIDIBUTTON 10023
#define ID_BANKTEXT 10025
#define ID_SETTINGSBUTTON 10026
#define ID_VOLUME_LEFT 10027
#define ID_VOLUME_RIGHT 10028
#define ID_FILTERBUTTON 10029
#define ID_ADSRBUTTON 10030

/*!
 * Compatibility
 */
#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * wxKeyboard class declaration
 */
class wxKeyboard: public wxDialog, public wxThread, public OctaveCallback, public MidiSettingsInterface
{
    DECLARE_DYNAMIC_CLASS( wxKeyboard )
    DECLARE_EVENT_TABLE()
public:
    /// Constructors
    wxKeyboard( );
    wxKeyboard( wxWindow* parent, wxWindowID id = SYMBOL_WXKEYBOARD_IDNAME, const wxString& caption = SYMBOL_WXKEYBOARD_TITLE, const wxPoint& pos = SYMBOL_WXKEYBOARD_POSITION, const wxSize& size = SYMBOL_WXKEYBOARD_SIZE, long style = SYMBOL_WXKEYBOARD_STYLE );
	virtual ~wxKeyboard();
    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WXKEYBOARD_IDNAME, const wxString& caption = SYMBOL_WXKEYBOARD_TITLE, const wxPoint& pos = SYMBOL_WXKEYBOARD_POSITION, const wxSize& size = SYMBOL_WXKEYBOARD_SIZE, long style = SYMBOL_WXKEYBOARD_STYLE );
    void CreateControls();
    void OnCloseWindow( wxCloseEvent& event );
    void OnKeyDown( wxKeyEvent& event );
    void OnKeyUp( wxKeyEvent& event );
    void OnBankSpinUp( wxSpinEvent& event );
    void OnBankSpinDown( wxSpinEvent& event );
    void OnPatchSpinUp( wxSpinEvent& event );
    void OnPatchSpinDown( wxSpinEvent& event );
	void OnMidiSettings( wxCommandEvent& event );
	void OnPitchWheel( wxScrollEvent& event );
	void OnModWheel( wxScrollEvent& event );
	void OnPanic( wxCommandEvent& event );
	void OnInfo( wxCommandEvent& event );
	void OnHelp( wxCommandEvent& event );
	void OnFilter( wxCommandEvent& event );
	void OnAdsr( wxCommandEvent& event );
	void OnLoadConfig( wxCommandEvent& event );
	void OnSaveConfig( wxCommandEvent& event );
	void OnSettings( wxCommandEvent& event );
	int GetOctaveByNote( int note );
	int GetNoteValue( int value );
	void NoteOff( int note );
	void AllControllersOff( void );
	void AllSoundOff( void );
	void SetPatch( int patch, bool receivedFromMidi = false );
	void SetBank( int bank, bool receivedFromMidi = false );
	void ShowMappedSamples( bool show );
    void OnMouseRelease( wxMouseEvent &event );
    wxBitmap GetBitmapResource( const wxString& name );
    wxIcon GetIconResource( const wxString& name );
    static bool ShowToolTips();
	int GetNumNotesPlaying();
	void* Entry();
	float GetEnvelopeVolume(int note, int playbackSamplePosition);
	// OctaveCallback members.
	void PlayNote( int note, bool receivedFromMidi = false );
	void StopNote( int note, bool receivedFromMidi = false );
	void AllNotesOff( bool receivedFromMidi = false );
	void OnRightClick( int note );
	// End OctaveCallback members.
	void RefreshSampleData();
	bool LoadSample(int sampleNumber);
	void ClearSampleData(int sampleNumber);
	void CopySampleData(int start, int end, int source);
	// MidiSettingsInterface methods.
	void SelectMidiInputDevice(int number);
	void SelectMidiOutputDevice(int number);
	void SelectMidiInputChannel(int number);
	void SelectMidiOutputChannel(int number);
	void EnableMidiOutput(bool enabled);
	// End MidiSettingsInterface methods.
	void SendMidiMessage( unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4, bool shortmsg = false );
	void ProcessMidiMessage(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4);
private:
	int _numOctaves;
	wxSettingsFile* _settingsFile;
	wxStaticText* _bankText;
	wxStaticText* _patchText;
    wxBitmapSpinButton* _bankSpin;
    wxBitmapSpinButton* _patchSpin;
	wxKeylessBitmapButton* _panicButton;
	wxKeylessBitmapButton* _infoButton;
	wxKeylessBitmapButton* _helpButton;
	wxKeylessBitmapButton* _saveButton;
	wxKeylessBitmapButton* _loadButton;
	wxKeylessBitmapButton* _midiButton;
	wxKeylessBitmapButton* _filterButton;
	wxKeylessBitmapButton* _settingsButton;
	wxKeylessBitmapButton* _adsrButton;
    wxBitmapSlider* _pitchWheel;
    wxBitmapSlider* _modWheel;
	wxVolumeMeter* _volumeMeterL;
	wxVolumeMeter* _volumeMeterR;
    wxColour _textColour;
    wxColour _backgroundColour;
	wxHtmlHelpController* _helpCtrl;
    int _inputChannel;
	int _outputChannel;
	int _midiInputDeviceNumber;
	int _midiOutputDeviceNumber;
    int _ipatch;
    int _ibank;
	int _bufferSize; // Buffer size in number of milliseconds.
    SampleData* _sample[MAX_NOTES];
    bool _playing[MAX_NOTES];
    wxOctaveCtrl* _octave[MAX_OCTAVES];
	bool _midiOutputEnabled;
	wxIcon _icon;
	AdsrParameters _adsrParameters;
#ifdef WIN32
	XAUDIO2_FILTER_PARAMETERS _filterParameters;
	IUnknown * _volumeMeterAPO;
	IUnknown * _reverbAPO;
	IXAudio2* _xAudio2;
	IXAudio2MasteringVoice* _masteringVoice;
#endif
	// Do NOT declare these, they're inherited from MidiSettingsInterface.
	//RtMidiOut* _midiOutDevice;
    //RtMidiIn* _midiInDevice;
};

#endif
    // _WXKEYBOARD_H_
