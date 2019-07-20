#include "wx/wx.h"
#include "SamplitronApp.h"

/*!
 * Application instance implementation
 */
IMPLEMENT_APP( SamplitronApp )

/*!
 * SamplitronApp type definition
 */
IMPLEMENT_CLASS( SamplitronApp, wxApp )

/*!
 * SamplitronApp event table definition
 */
BEGIN_EVENT_TABLE( SamplitronApp, wxApp )
END_EVENT_TABLE()

/*!
 * Constructor for SamplitronApp
 */
SamplitronApp::SamplitronApp()
{
}

/*!
 * Initialisation for SamplitronApp
 */
bool SamplitronApp::OnInit()
{
    wxInitAllImageHandlers();
    Samplitron* mainWindow = new Samplitron(NULL, ID_KEYBOARD_DLG );
    mainWindow->Show(true);
    return true;
}

/*!
 * Cleanup for SamplitronApp
 */
int SamplitronApp::OnExit()
{
    return wxApp::OnExit();
}

