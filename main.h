/***************************************************************
 * Name:      jugaadMain.h
 * Purpose:   Defines Application Frame
 * Author:    Robins (robinskumar73@gmail.com)
 * Created:   2014-06-23
 * Copyright: Robins ()
 * License:
 **************************************************************/

#ifndef JUGAADMAIN_H
#define JUGAADMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "app.h"

class jugaadFrame: public wxFrame
{
    public:
        jugaadFrame(wxFrame *frame, const wxString& title);
        ~jugaadFrame();
    private:
        enum
        {
            idMenuQuit = 1000,
            idMenuAbout
        };
        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        DECLARE_EVENT_TABLE()
};


#endif // JUGAADMAIN_H
