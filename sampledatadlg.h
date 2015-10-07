#ifndef _SAMPLEDATADLG_H_
#define _SAMPLEDATADLG_H_

// wxWidgets dialog used for editing sample parameters attached to a particular key.

#include "sampledata.h"
#include "wx/wx.h"
#include "wx/spinctrl.h"

#define ID_SAMPLEDATA_DLG 13000
#define SYMBOL_SAMPLEDATADLG_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_SAMPLEDATADLG_TITLE _("Edit Sample Information")
#define SYMBOL_SAMPLEDATADLG_IDNAME ID_SAMPLEDATA_DLG
#define SYMBOL_SAMPLEDATADLG_SIZE wxSize(400, 440)
#define SYMBOL_SAMPLEDATADLG_POSITION wxDefaultPosition
#define ID_SPIN_SAMPLENOTE 13001
#define ID_SPIN_KEYBOARDNOTE 13002
#define ID_SPIN_VOLUME 13003
#define ID_BUTTON_BROWSE 13004
#define ID_TEXT_FILENAME 13005
#define ID_BUTTON_OK 13006
#define ID_BUTTON_CLEAR 13007
#define ID_CHECK_LOOP 13008

class SampleDataDlg : public wxDialog
{
    DECLARE_DYNAMIC_CLASS( SampleDataDlg )
    DECLARE_EVENT_TABLE()
public:
    SampleDataDlg();
    ~SampleDataDlg();
    SampleDataDlg( wxWindow* parent, wxWindowID id = SYMBOL_SAMPLEDATADLG_IDNAME, const wxString& caption = SYMBOL_SAMPLEDATADLG_TITLE, const wxPoint& pos = SYMBOL_SAMPLEDATADLG_POSITION, const wxSize& size = SYMBOL_SAMPLEDATADLG_SIZE, long style = SYMBOL_SAMPLEDATADLG_STYLE );
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SAMPLEDATADLG_IDNAME, const wxString& caption = SYMBOL_SAMPLEDATADLG_TITLE, const wxPoint& pos = SYMBOL_SAMPLEDATADLG_POSITION, const wxSize& size = SYMBOL_SAMPLEDATADLG_SIZE, long style = SYMBOL_SAMPLEDATADLG_STYLE );
    void CreateControls();
    void OnOK( wxCommandEvent& event );
    void OnClose( wxCloseEvent& event );
    void OnBrowse( wxCommandEvent& event );
	void OnClear( wxCommandEvent& event );
	void SetSample(SampleData * sample );
    SampleData* GetSample(void);
	void UpdateSemitones( wxCommandEvent& event );
	void UpdateSemitones( wxSpinEvent& event );
	void UpdateSemitones();
private:
    wxSpinCtrl* _spnSampleNote;
    wxSpinCtrl* _spnKeyboardNote;
    wxTextCtrl* _txtFilename;
    wxSpinCtrl* _spnVolume;
	wxCheckBox* _chkLoop;
    wxButton* _btnBrowse;
    wxButton* _btnClose;
	wxButton* _btnClear;
    wxColour _backgroundColour;
    wxColour _textColour;
	wxStaticText* _resampleSemitones;
};

#endif
