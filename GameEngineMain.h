/***************************************************************
 * Name:      GameEngineMain.h
 * Purpose:   Defines Application Frame
 * Author:    Jack Pettett ()
 * Created:   2017-05-12
 * Copyright: Jack Pettett (turquiosesymphony.com)
 * License:
 **************************************************************/

#ifndef GAMEENGINEMAIN_H
#define GAMEENGINEMAIN_H

#include <wx/textfile.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>

//(*Headers(GameEngineFrame)
#include <wx/stattext.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/frame.h>
//*)

class GameEngineFrame: public wxFrame
{
    public:

        GameEngineFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~GameEngineFrame();
        void OnKeyDown(wxKeyEvent& event);
        void OnLoad();
        void RenderMain(wxDC& dc);
        void LoadIntro();
        void InitiateNewGame();
        void DrawGame();
        void RetrieveFrame(GameEngineFrame* MyFrame);
        void MainGameLoop();
        void CalledPerTick();
        bool ProcessEvents();
        void CreatePlayer();
        void CreateTargets();
        void CreateTopDownLevel();
        void SCalledPerTick();
        void SProcessEvents();
        void SDrawGame();
        void SAutoMove();
        void SCreateLevel();
        void CloseGate();
        bool AutoAIMove();
        void CreateBullet();
        void DrawEditor();
    private:

        //(*Handlers(GameEngineFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnButton1Click(wxCommandEvent& event);
        void OnPanel1Paint(wxPaintEvent& event);
        void OnFirstPaint(wxPaintEvent& event);
        void OnButton5Click(wxCommandEvent& event);
        void OnButton4Click(wxCommandEvent& event);
        void OnButton7Click(wxCommandEvent& event);
        void OnPanel1LeftDown(wxMouseEvent& event);
        void OnButton8Click(wxCommandEvent& event);
        void OnPanel1Resize(wxSizeEvent& event);
        void OnSetFocus(wxFocusEvent& event);
        void OnSetFocus1(wxFocusEvent& event);
        void OnKillFocus(wxFocusEvent& event);
        void OnButton6Click(wxCommandEvent& event);
        void OnPanel1RightDown(wxMouseEvent& event);
        //*)

        //(*Identifiers(GameEngineFrame)
        static const long ID_BUTTON1;
        static const long ID_BUTTON2;
        static const long ID_BUTTON3;
        static const long ID_BUTTON4;
        static const long ID_BUTTON5;
        static const long ID_BUTTON6;
        static const long ID_STATICTEXT1;
        static const long ID_BUTTON7;
        static const long ID_STATICTEXT2;
        static const long ID_BUTTON8;
        static const long ID_STATICTEXT3;
        static const long ID_STATICTEXT4;
        static const long ID_STATICTEXT5;
        static const long ID_PANEL1;
        //*)

        //(*Declarations(GameEngineFrame)
        wxStaticText* PlayerYLabel;
        wxButton* Button4;
        wxStaticText* PlayerXLabel;
        wxButton* Button1;
        wxPanel* Panel1;
        wxButton* Button2;
        wxButton* Button6;
        wxButton* Button5;
        wxButton* Button3;
        wxButton* Button7;
        wxStaticText* IntroText;
        wxStaticText* HasSelectedAI;
        wxStaticText* TickCounterLabel;
        wxButton* Button8;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // GAMEENGINEMAIN_H
