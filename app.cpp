/***************************************************************
 * Name:      jugaadApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Robins (robinskumar73@gmail.com)
 * Created:   2014-06-23
 * Copyright: Robins ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "app.h"
#include "main.h"


IMPLEMENT_APP(jugaadApp);

bool jugaadApp::OnInit()
{
    jugaadFrame* frame = new jugaadFrame(0L, _("wxWidgets Application Template"));
    frame->SetIcon(wxICON(aaaa)); // To Set App Icon
    frame->Show();

    return true;
}

