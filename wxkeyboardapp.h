#ifndef _WXKEYBOARDAPP_H_
#define _WXKEYBOARDAPP_H_

/*!
 * Includes
 */
#include "wx/image.h"
#include "wxkeyboard.h"

/*!
 * wxKeyboardApp class declaration
 */

class wxKeyboardApp: public wxApp
{
    DECLARE_CLASS( wxKeyboardApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    wxKeyboardApp();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();
};

/*!
 * Application instance declaration 
 */

DECLARE_APP(wxKeyboardApp)

#endif
