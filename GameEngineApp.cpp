/***************************************************************
 * Name:      GameEngineApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Jack Pettett ()
 * Created:   2017-05-12
 * Copyright: Jack Pettett (turquiosesymphony.com)
 * License:
 **************************************************************/

#include "wx_pch.h"
#include "GameEngineApp.h"

//(*AppHeaders
#include "GameEngineMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(GameEngineApp);

GameEngineFrame* Frame;

bool GameEngineApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	Frame = new GameEngineFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;
}

GameEngineFrame* FunctionToSendPtrToFrame()
{
    return Frame;
}
