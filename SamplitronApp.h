#ifndef _SAMPLITRONAPP_H_
#define _SAMPLITRONAPP_H_

/*!
 * Includes
 */
#include "wx/image.h"
#include "Samplitron.h"

/*!
 * SamplitronApp class declaration
 */

class SamplitronApp: public wxApp
{
    DECLARE_CLASS( SamplitronApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    SamplitronApp();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();
};

/*!
 * Application instance declaration 
 */

DECLARE_APP(SamplitronApp)

#endif
