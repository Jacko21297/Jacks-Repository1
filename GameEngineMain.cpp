/***************************************************************
 * Name:      GameEngineMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Jack Pettett ()
 * Created:   2017-05-12
 * Copyright: Jack Pettett (turquiosesymphony.com)
 * License:
 **************************************************************/

 /**

 //Use extern to declare global variables that are also declared or initialised elsewhere.

 Currently you need to do:

 - Loading Screen with main menu GUI
 - Main Menu Screen needs move buttons to the middle of the screen.
 - A Level's Default Generation, perhaps include in the constructor for a level, the automatic creation of the Vector Grid. This function is half implemented, just move it to the constructor.
 - Ability to add events to the Event stack vector, this vector exists you just need to add TO it.
 - There needs to be a text file or similar that can store game save data for future access.
 - You need to Implement The ability to import bitmap images.
 - Create a derived class that holds a bitmap (image, graphic ID), and ::Location. then create a vector of this class to allow easy access for drawing graphics when you have implemented them.
 - You also need to Implement the ability to include audio files?
 - You need to have the main game loop functioning.
 - Create a map editor.(Do this kinda last, cos that screwed you by trying do it before you had the actual Level creation Concept.
 - Include Double Buffered DC's when drawing graphics.
 - The Game Loop Should be similar to this in some way. Handle events should take the last element off the Event Vector.
 - MAKE SURE YOU ARE ALWAYS ADDING THE MOST RECENTLY CALLED EVENT TO THE BEGGINING OF THE VECTOR, NOT THE END. Do a for loop that iterates through the elements currently in the stack
 so it can place the most recent one at the end.

 Something like this might work:

 Event position number = 5;

 Vector = 4 3 2 1
 SortingVector = empty;

 for(int x=Vector.Size(); x>=0; x--)
 {
    SortingVector.push_back(Vector.back());
    Vector.Pop_back;
 }

Sorting Vector would = 1 2 3 4

Sorting Vector.Push_Back(New Int);

Sorting Vector would = 1 2 3 4 5

 for(int x=SortingVector.Size(); x>=0; x--)
 {
    Vector.push_back(SortingVector.back());
    SortingVector.Pop_back();
 }

 Vector would = 5 4 3 2 1
 */

 //////////////////////////////////////////////////////////////////////

 /**
 Potential Game Loop should be something like this:
  // initialize the engine
  game.Init( "Engine Test v1.0" );

  // load the intro
  game.ChangeState( CIntroState::Instance() );

  // main loop
  while ( game.Running() )
  {
    game.HandleEvents();
    game.Update();
    game.Draw();
  }

  // cleanup the engine
  game.Cleanup();
  return 0;
}


 */

#include "wx_pch.h"
#include "wx/wx.h"
#include "GameEngineMain.h"
#include <wx/msgdlg.h>
#include <vector>

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "Tile.h"
#include "Block.h"
#include "Player.h"
#include "Item.h"
#include "Level.h"
#include "LevelGeneration.h"

void CreateLevel();

std::vector<int> EventStack; //All events get pushed onto this stack to be dealt with by the Custom Event Handler.
std::vector<int> SEventStack; //Stores events for the side scroller.
std::vector<wxButton*> MainButtons; //stores buttons for main menu.
std::vector<wxButton*> MainButtonStorage; //Stores Buttons for after use transfer.
std::vector<wxBitmap> ImageStorage; //Stores all images required during applications use.
std::vector<wxBitmap> PlayerImage;
std::vector<wxBitmap> SPlayerImage;
std::vector<wxBitmap> AIImage;
std::vector<wxBitmap> SImage;
std::vector<wxBitmap> BImage;

std::vector<wxBitmap> SImageStorage;

std::vector<std::vector<Block>> GridEditor;

Level LevelOne;
Level SideScroller;
Player Me("Jack");
TargetAI TargetOne;
wxSize ScreenSize; //Used to store the users screen size.
wxSize WindowSize; //Used to store the windows size.

bool MainMenu = true;
bool GameOver = false;
bool InEditor = false;
bool WithinGameLoadingScreen = false;
bool WithinGame = false;
bool GamePaused = false;
bool InTopDown = false;
bool InSideScroller = false;
bool FirstSwitch = true;
bool LevelCreated = false;

int NumberOfFittingCellsX, NumberOfFittingCellsY;

int RemainderX, RemainderY;

int IncrementX = 32;
int IncrementY = 32;

int OriginalUpperY;
int OriginalUpperX;

int SelectedImage;

wxString DefaultPath = "D:\\CodeBlocksData\\CodeBlocksCodeStuff\\GameEngine\\Graphics\\";

//(*InternalHeaders(GameEngineFrame)
#include <wx/settings.h>
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(GameEngineFrame)
const long GameEngineFrame::ID_BUTTON1 = wxNewId();
const long GameEngineFrame::ID_BUTTON2 = wxNewId();
const long GameEngineFrame::ID_BUTTON3 = wxNewId();
const long GameEngineFrame::ID_BUTTON4 = wxNewId();
const long GameEngineFrame::ID_BUTTON5 = wxNewId();
const long GameEngineFrame::ID_BUTTON6 = wxNewId();
const long GameEngineFrame::ID_STATICTEXT1 = wxNewId();
const long GameEngineFrame::ID_BUTTON7 = wxNewId();
const long GameEngineFrame::ID_STATICTEXT2 = wxNewId();
const long GameEngineFrame::ID_BUTTON8 = wxNewId();
const long GameEngineFrame::ID_STATICTEXT3 = wxNewId();
const long GameEngineFrame::ID_STATICTEXT4 = wxNewId();
const long GameEngineFrame::ID_STATICTEXT5 = wxNewId();
const long GameEngineFrame::ID_PANEL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(GameEngineFrame,wxFrame)
    //(*EventTable(GameEngineFrame)
    //*)
END_EVENT_TABLE()

GameEngineFrame::GameEngineFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(GameEngineFrame)
    Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(256,304), wxDefaultSize, wxTAB_TRAVERSAL|wxNO_FULL_REPAINT_ON_RESIZE, _T("ID_PANEL1"));
    Button1 = new wxButton(Panel1, ID_BUTTON1, _("Start Game"), wxPoint(184,280), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    Button2 = new wxButton(Panel1, ID_BUTTON2, _("Load Game"), wxPoint(184,304), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    Button3 = new wxButton(Panel1, ID_BUTTON3, _("Help"), wxPoint(184,328), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    Button4 = new wxButton(Panel1, ID_BUTTON4, _("Options"), wxPoint(184,352), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
    Button5 = new wxButton(Panel1, ID_BUTTON5, _("Exit"), wxPoint(184,376), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON5"));
    Button6 = new wxButton(Panel1, ID_BUTTON6, _("Map Editor"), wxPoint(184,408), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON6"));
    IntroText = new wxStaticText(Panel1, ID_STATICTEXT1, _("Place Holder Text"), wxPoint(64,152), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    IntroText->Hide();
    IntroText->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
    IntroText->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_CAPTIONTEXT));
    wxFont IntroTextFont(48,wxFONTFAMILY_SWISS,wxFONTSTYLE_ITALIC,wxFONTWEIGHT_BOLD,false,_T("Armalite Rifle"),wxFONTENCODING_DEFAULT);
    IntroText->SetFont(IntroTextFont);
    Button7 = new wxButton(Panel1, ID_BUTTON7, _("Stop"), wxPoint(88,360), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON7"));
    TickCounterLabel = new wxStaticText(Panel1, ID_STATICTEXT2, _("0"), wxPoint(128,104), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    Button8 = new wxButton(Panel1, ID_BUTTON8, _("Label"), wxPoint(96,320), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON8"));
    PlayerXLabel = new wxStaticText(Panel1, ID_STATICTEXT3, _("Label"), wxPoint(72,56), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    PlayerYLabel = new wxStaticText(Panel1, ID_STATICTEXT4, _("Label"), wxPoint(72,72), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    HasSelectedAI = new wxStaticText(Panel1, ID_STATICTEXT5, _("Nope"), wxPoint(136,72), wxDefaultSize, 0, _T("ID_STATICTEXT5"));

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&GameEngineFrame::OnButton1Click);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&GameEngineFrame::OnButton4Click);
    Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&GameEngineFrame::OnButton5Click);
    Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&GameEngineFrame::OnButton6Click);
    Connect(ID_BUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&GameEngineFrame::OnButton7Click);
    Connect(ID_BUTTON8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&GameEngineFrame::OnButton8Click);
    Panel1->Connect(wxEVT_PAINT,(wxObjectEventFunction)&GameEngineFrame::OnPanel1Paint,0,this);
    Panel1->Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&GameEngineFrame::OnPanel1LeftDown,0,this);
    Panel1->Connect(wxEVT_RIGHT_DOWN,(wxObjectEventFunction)&GameEngineFrame::OnPanel1RightDown,0,this);
    Panel1->Connect(wxEVT_SIZE,(wxObjectEventFunction)&GameEngineFrame::OnPanel1Resize,0,this);
    //*)

    OnLoad();

    Panel1->Bind(wxEVT_CHAR_HOOK, &GameEngineFrame::OnKeyDown, this);

    CreateLevel();
    CreateTopDownLevel();
    CreatePlayer();
    CreateTargets();

}

void GameEngineFrame::OnLoad()
{

    srand (time(NULL));
    ScreenSize.SetHeight(wxSystemSettings::GetMetric( wxSYS_SCREEN_Y)-72);
    ScreenSize.SetWidth(wxSystemSettings::GetMetric( wxSYS_SCREEN_X)-2);

    this->SetClientSize(ScreenSize.GetWidth(), ScreenSize.GetHeight());

    wxPoint StartPosition;
    StartPosition.x = -7;
    StartPosition.y = 0;
    this->SetPosition(StartPosition);

    WindowSize.SetHeight(GetClientSize().GetHeight());
    WindowSize.SetWidth(GetClientSize().GetWidth());

    RemainderX = WindowSize.GetWidth() % 64;
    RemainderY = WindowSize.GetHeight() % 64;

    NumberOfFittingCellsX = (WindowSize.GetWidth() - RemainderX) / 64;
    NumberOfFittingCellsY = (WindowSize.GetHeight() - RemainderY) / 64;

    OriginalUpperX = NumberOfFittingCellsX;
    OriginalUpperY = NumberOfFittingCellsY;

    LevelOne.SetUpperX(NumberOfFittingCellsX);
    LevelOne.SetUpperY(NumberOfFittingCellsY);

    SideScroller.SetUpperX(NumberOfFittingCellsX);
    SideScroller.SetUpperY(NumberOfFittingCellsY);

    MainButtons.push_back(Button6);
    MainButtons.push_back(Button5);
    MainButtons.push_back(Button4);
    MainButtons.push_back(Button3);
    MainButtons.push_back(Button2);
    MainButtons.push_back(Button1);

    wxBitmap Default(wxBitmap(wxImage(DefaultPath + "PlaceHolderTile.bmp")));
    wxBitmap DefaultWall(wxBitmap(wxImage(DefaultPath + "DefaultWall.bmp")));
    wxBitmap Floor(wxBitmap(wxImage(DefaultPath + "TopDownGrass.bmp")));
    wxBitmap Water(wxBitmap(wxImage(DefaultPath + "Water.bmp")));
    wxBitmap CornerWall(wxBitmap(wxImage(DefaultPath + "CornerWall.bmp")));

    wxBitmap PlayerFront(wxBitmap(wxImage(DefaultPath + "FrontWitch.bmp")));
    wxBitmap PlayerBack(wxBitmap(wxImage(DefaultPath + "BackWitch.bmp")));
    wxBitmap PlayerRight(wxBitmap(wxImage(DefaultPath + "RightWitch.bmp")));
    wxBitmap PlayerLeft(wxBitmap(wxImage(DefaultPath + "LeftWitch.bmp")));

    wxBitmap SPlayerFront(wxBitmap(wxImage(DefaultPath + "SFrontWitch.bmp")));
    wxBitmap SPlayerRight(wxBitmap(wxImage(DefaultPath + "SRightWitch.bmp")));
    wxBitmap SPlayerLeft(wxBitmap(wxImage(DefaultPath + "SLeftWitch.bmp")));

    wxBitmap RogueFrontGrass(wxBitmap(wxImage(DefaultPath + "RogueFrontGrass.bmp")));
    wxBitmap RogueLeftAir(wxBitmap(wxImage(DefaultPath + "RogueLeftAir.bmp")));
    wxBitmap RogueRightAir(wxBitmap(wxImage(DefaultPath + "RogueRightAir.bmp")));
    wxBitmap RogueFrontAir(wxBitmap(wxImage(DefaultPath + "RogueFrontAir.bmp")));
    wxBitmap RogueLeftGunAir(wxBitmap(wxImage(DefaultPath + "RogueLeftGunAir.bmp")));
    wxBitmap RogueLeftGunShootAir(wxBitmap(wxImage(DefaultPath + "RogueLeftGunShootingAir.bmp")));
    wxBitmap RogueLeftGunSmokeAir(wxBitmap(wxImage(DefaultPath + "RogueLeftGunSmokeAir.bmp")));


    wxBitmap Air(wxBitmap(wxImage(DefaultPath + "Air.bmp")));
    wxBitmap Grass(wxBitmap(wxImage(DefaultPath + "Grass.bmp")));
    wxBitmap Dirt(wxBitmap(wxImage(DefaultPath + "Dirt.bmp")));
    wxBitmap RedBrick(wxBitmap(wxImage(DefaultPath + "RedBricks.bmp")));
    wxBitmap LeverOff(wxBitmap(wxImage(DefaultPath + "AirLeverOff.bmp")));
    wxBitmap LeverOn(wxBitmap(wxImage(DefaultPath + "AirLeverOn.bmp")));

    wxBitmap BulletAir(wxBitmap(wxImage(DefaultPath + "BulletAir.bmp")));

    BImage.push_back(BulletAir);

    ImageStorage.push_back(Floor);
    ImageStorage.push_back(DefaultWall);
    ImageStorage.push_back(Default);
    ImageStorage.push_back(RedBrick);
    ImageStorage.push_back(Dirt);
    ImageStorage.push_back(Water);
    ImageStorage.push_back(CornerWall);

    PlayerImage.push_back(PlayerBack);
    PlayerImage.push_back(PlayerFront);
    PlayerImage.push_back(PlayerLeft);
    PlayerImage.push_back(PlayerRight);

    SPlayerImage.push_back(SPlayerFront);
    SPlayerImage.push_back(SPlayerRight);
    SPlayerImage.push_back(SPlayerLeft);

    AIImage.push_back(RogueFrontGrass);

    SImage.push_back(RogueRightAir);
    SImage.push_back(RogueFrontAir);
    SImage.push_back(RogueLeftAir);
    SImage.push_back(RogueLeftGunAir);
    SImage.push_back(RogueLeftGunShootAir);
    SImage.push_back(RogueLeftGunSmokeAir);

    SImageStorage.push_back(Air);
    SImageStorage.push_back(Grass);
    SImageStorage.push_back(Dirt);
    SImageStorage.push_back(RedBrick);
    SImageStorage.push_back(LeverOff);
    SImageStorage.push_back(LeverOn);

}

void GameEngineFrame::RenderMain(wxDC& dc)
{
    WindowSize.SetHeight(GetClientSize().GetHeight());
    WindowSize.SetWidth(GetClientSize().GetWidth());

    wxPoint BtnP;

    BtnP.y = ((WindowSize.GetHeight()/2) - (Button1->GetSize().y/2)- 75);
    BtnP.x = (WindowSize.GetWidth()/2) - (Button1->GetSize().x/2);

    MainButtonStorage = MainButtons;

    int Spacer = Button1->GetSize().GetY();

    for(int x=0; x<6; x++)
    {
        MainButtons.back()->SetPosition(BtnP);
        MainButtons.pop_back();
        BtnP.y += (Spacer);
    }

    MainButtons = MainButtonStorage;
    if(MainMenu)
    {
        wxColor Colour;

        wxBitmap Background(WindowSize.GetWidth(), WindowSize.GetHeight());
        wxMemoryDC *MDC = new wxMemoryDC; //Create a pointer to a new memory device context

        MDC->SelectObject(Background); //select the empty bitmap
        //MyMemDC.SelectObject(Background);

        MDC->Clear();

        int Red, Green, Blue;

    /// 0,0,0 / 1,0,0 / 2,0,0 / 3,0,0 / 0,1,0 / 0,2,0, / 0,3,0 / 0,0,1 / 0,0,2 / 0,0,3
    /// 1,1,0 / 1,2,0 / 1,3,0 / 2,1,0 / 2,2,0 / 2,3,0 / 3,1,0 / 3,2,0 / 3,3,0 /

        MDC->SetPen(Colour);

        srand (time(NULL));

    /*
            Red = rand() % 255 + 1;
            Green = (Green*500) % 255 + 1;
            Blue = (Blue*Red) % 255 + 1;

            Red = PBlue*999 % 255 + 1;
            Green = PRed*333 % 255 + 1;
            Blue =  PGreen*999 % 255 + 1;
    */

        for(int y=0; y<WindowSize.GetHeight(); y++){

            Red = rand() % 255 + 105;
            Green = rand() % 255 + 105;
            Blue = rand() % 155 + 105;
            Colour.Set(Red, Green, Blue);
            MDC->SetPen(Colour);
            MDC->SetBrush(Colour);

            MDC->DrawLine(0, y, WindowSize.GetWidth(), y);

        }

        MDC->SelectObject(wxNullBitmap); //Free memory for MDC
        delete MDC; //Delete MDC

        dc.DrawBitmap(Background, 0 ,0); //Draw the bitmap that was created previously.
    }
}

GameEngineFrame::~GameEngineFrame()
{
    GamePaused = true;
    GameOver = true;
    //(*Destroy(GameEngineFrame)
    //*)
}

void GameEngineFrame::OnQuit(wxCommandEvent& event)
{
    GamePaused = true;
    GameOver = true;
    Close();
}

void GameEngineFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void GameEngineFrame::OnKeyDown(wxKeyEvent& event)
{
    //wxMessageBox(wxString::Format("KeyDown: %i\n", (int)event.GetKeyCode()));

    std::vector<int> SortingStack;

    if(WithinGame)
    {
        if(InTopDown)
        {

            int CurrentUpperX, CurrentUpperY, CurrentLowerX, CurrentLowerY;

            switch((int)event.GetKeyCode())
            {
            case 65:
                //wxMessageBox("A");
                SortingStack.push_back(3);
                for(int x=EventStack.size()-1; x >= 0; x--)
                {
                    SortingStack.push_back(EventStack.back());
                    EventStack.pop_back();
                }

                EventStack.clear();
                EventStack = SortingStack;
                break;

            case 68:
                //wxMessageBox("D");
                SortingStack.push_back(4);
                for(int x=EventStack.size()-1; x >= 0; x--)
                {
                    SortingStack.push_back(EventStack.back());
                    EventStack.pop_back();
                }

                EventStack.clear();
                EventStack = SortingStack;
                break;

            case 83:
                //wxMessageBox("S");
                SortingStack.push_back(2);
                for(int x=EventStack.size()-1; x >= 0; x--)
                {
                    SortingStack.push_back(EventStack.back());
                    EventStack.pop_back();
                }

                EventStack.clear();
                EventStack = SortingStack;
                break;

            case 87:
                //wxMessageBox("W");
                SortingStack.push_back(1);
                for(int x=EventStack.size()-1; x >= 0; x--)
                {
                    SortingStack.push_back(EventStack.back());
                    EventStack.pop_back();
                }

                EventStack.clear();
                EventStack = SortingStack;
                break;

            case 32:
                SortingStack.push_back(5);
                for(int x=EventStack.size()-1; x >= 0; x--)
                {
                    SortingStack.push_back(EventStack.back());
                    EventStack.pop_back();
                }

                EventStack.clear();
                EventStack = SortingStack;
                break;

            case 314:
                //wxMessageBox("Left");
                break;

            case 315:
            //wxMessageBox("Up");
                break;

            case 316:
            //wxMessageBox("Right");
                break;

            case 317:
            //wxMessageBox("Down");
                break;

            case 27:
            //wxMessageBox("ESC")
                if(!GamePaused)
                {
                    GamePaused = true;
                }
                else
                {
                    GamePaused = false;
                }
                break;

            default:
                wxMessageBox(wxString::Format("Code: %i ", (int)event.GetKeyCode()));
                break;

            }


        }
        else if(InSideScroller)
        {

            switch((int)event.GetKeyCode())
            {
            case 65:
                //wxMessageBox("A");
                SortingStack.push_back(2);
                for(int x=SEventStack.size()-1; x >= 0; x--)
                {
                    SortingStack.push_back(SEventStack.back());
                    SEventStack.pop_back();
                }

                SEventStack.clear();
                SEventStack = SortingStack;
                break;

            case 68:
                //wxMessageBox("D");
                SortingStack.push_back(1);
                for(int x=SEventStack.size()-1; x >= 0; x--)
                {
                    SortingStack.push_back(SEventStack.back());
                    SEventStack.pop_back();
                }

                SEventStack.clear();
                SEventStack = SortingStack;
                break;

            case 32:
                //wxMessageBox("Space");
                SortingStack.push_back(3);
                for(int x=SEventStack.size()-1; x >= 0; x--)
                {
                    SortingStack.push_back(SEventStack.back());
                    SEventStack.pop_back();
                }

                SEventStack.clear();
                SEventStack = SortingStack;
                break;

            default:
                break;

            }

        }

    }
    else if(InEditor)
        {
            switch((int)event.GetKeyCode())
            {
            case 65:
                //wxMessageBox("A");
                if(LevelOne.GetLowerX() > 0)
                {
                    LevelOne.SetLowerX(LevelOne.GetLowerX()-1);
                    LevelOne.SetUpperX(LevelOne.GetUpperX()-1);
                    DrawEditor();
                }

                break;

            case 68:
                //wxMessageBox("D");
                if(LevelOne.GetUpperX() < LevelOne.GetMaxX())
                {
                    LevelOne.SetLowerX(LevelOne.GetLowerX()+1);
                    LevelOne.SetUpperX(LevelOne.GetUpperX()+1);
                    DrawEditor();
                }


                break;

            case 83:
                //wxMessageBox("S");

                break;

            case 87:
                //wxMessageBox("W");

                break;

            case 32:
                //Save ?
                break;
            default:
                break;

                ///Might be an issue when going from editor to game so be wary, just reset the values of lowerx and lowery etc
        }

    }
    event.Skip();

}

void GameEngineFrame::CreatePlayer()
{
    Me.SetPPosX(10);
    Me.SetPPosY(10);

    Me.SetPPrevX(10);
    Me.SetPPrevY(10);

    Me.SetPScreenX(10);
    Me.SetPScreenY(10);

    Me.SetDirection(1);
}

void GameEngineFrame::CreateTargets()
{
    TargetOne.SetAPosX(20);
    TargetOne.SetAPosY(4);
    TargetOne.SetHealth(100);
    TargetOne.SetName("Snitch Muggle");
}

void CreateLevel()
{
    wxTextFile file(wxT("MapDataFile.txt"));

        LevelOne.SetLevelID(1);
        LevelOne.SetLevelName("First Landing");
        LevelOne.SetSeed(7);
        LevelOne.SetMaxX(80);
        LevelOne.SetMaxY(80);
        LevelOne.SetLowerX(0);
        LevelOne.SetLowerY(0);
        LevelOne.SetUpperX(NumberOfFittingCellsX);
        LevelOne.SetUpperY(NumberOfFittingCellsY);

        std::vector<std::vector<Block>> Grid;

    if(!file.Exists())
    {
        wxString Line;

        //LevelOne.SetUpperX(NumberOfFittingCellsX);
        //LevelOne.SetUpperY(NumberOfFittingCellsY);

        wxString GridAsString = "";
        for(int x=0; x<LevelOne.GetMaxX(); x++){
            std::vector<Block> BRow;
            for(int y=0; y<LevelOne.GetMaxY(); y++){
                Block BlockToPush;
                BlockToPush.SetGraphicID(0);
                BlockToPush.SetHasPhysics(false);
                BlockToPush.SetIsPassable(true);
                BlockToPush.SetStringID("0");
                Line << BlockToPush.GetStringID();
                Line << " ";
                BlockToPush.SetPosX(x);
                BlockToPush.SetPosY(y);

                BRow.push_back(BlockToPush);

            }
            file.AddLine(Line);
            Line = "";
            Grid.push_back(BRow);
        }

        LevelOne.SetMapGrid(Grid);
        GridEditor = LevelOne.GetMapGrid();

        file.Write();
        file.Close();

    }
    else
    {
        file.Open(wxT("MapDataFile.txt"));
        wxFileInputStream input( "MapDataFile.txt" );
        wxTextInputStream text( input );

        wxString Singular;

        for(int x=0; x<LevelOne.GetMaxX(); x++){
            std::vector<Block> BRow;
            for(int y=0; y<LevelOne.GetMaxY(); y++){
                text >> Singular;
                Block BlockToPush;
                BlockToPush.SetGraphicID(wxAtoi(Singular));
                BlockToPush.SetHasPhysics(false);
                if(Singular == "0")BlockToPush.SetIsPassable(true);
                else BlockToPush.SetIsPassable(false);
                BlockToPush.SetStringID(Singular);
                BlockToPush.SetPosX(x);
                BlockToPush.SetPosY(y);

                BRow.push_back(BlockToPush);
                Singular = "";

            }
            Grid.push_back(BRow);
        }

        LevelOne.SetMapGrid(Grid);
        GridEditor = LevelOne.GetMapGrid();
        file.Close();

    }
    LevelCreated = true;

}

void GameEngineFrame::CreateTopDownLevel()
{
    std::vector<std::vector<Block>> Grid;

    SideScroller.SetLevelID(1);
    SideScroller.SetLevelName("The Chase");
    SideScroller.SetSeed(7);
    SideScroller.SetMaxX(30);
    SideScroller.SetMaxY(15);
    SideScroller.SetLowerX(0);
    SideScroller.SetLowerY(0);

    //SideScroller.SetUpperX(NumberOfFittingCellsX);
    //SideScroller.SetUpperY(NumberOfFittingCellsY);

    wxString GridAsString = "";
    for(int x=0; x<SideScroller.GetMaxX(); x++){
        std::vector<Block> BRow;
        std::vector<Item> IRow;
        for(int y=0; y<SideScroller.GetMaxY(); y++){
            Block BlockToPush;
            BlockToPush.SetGraphicID(0);
            BlockToPush.SetHasPhysics(false);
            BlockToPush.SetIsPassable(true);
            BlockToPush.SetStringID("0 ");
            BlockToPush.SetPosX(x);
            BlockToPush.SetPosY(y);

            if(y > 13)
            {
                BlockToPush.SetGraphicID(1);
                BlockToPush.SetIsPassable(false);
            }

            BRow.push_back(BlockToPush);

        }
        Grid.push_back(BRow);
    }

    SideScroller.SetMapGrid(Grid);
}

void GameEngineFrame::OnButton1Click(wxCommandEvent& event)
{
    InitiateNewGame();
}


void GameEngineFrame::OnPanel1Paint(wxPaintEvent& event)
{
    wxClientDC dc(Panel1);
    if(MainMenu)
    {
        RenderMain(dc);
    }

}

void GameEngineFrame::OnButton5Click(wxCommandEvent& event) //Exit Button
{
    wxMessageDialog *dial = new wxMessageDialog(NULL,
    wxT("Are you sure you wish to quit?"), wxT("Confirmation"),
    wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);

    int ret = dial->ShowModal();

    dial->Destroy();

    if (ret == wxID_YES) {
      Destroy();
    }
    else {

    }
}

void GameEngineFrame::OnButton4Click(wxCommandEvent& event)//Options Menu
{

}

void GameEngineFrame::OnButton7Click(wxCommandEvent& event)
{
    GamePaused = true;
    GameOver = true;

}

void GameEngineFrame::OnPanel1LeftDown(wxMouseEvent& event)
{
    if(WithinGameLoadingScreen)
    {
        WithinGameLoadingScreen = false;
    }

    const wxPoint pt = wxGetMousePosition();
    wxClientDC dc(Panel1);

    if(InEditor)
    {
        int mouseX = pt.x;
        int mouseY = pt.y;
        //wxMessageBox(wxString::Format("%i",(int)mouseX) + " " + wxString::Format("%i",(int)mouseY));

        int StartingX = 1601;
        int StartingY = 52;

        int IC = 64; //Increment value

        if((mouseX > StartingX && mouseX < StartingX+IC) && (mouseY > StartingY && mouseY < StartingY+IC))
        {
            dc.DrawBitmap(ImageStorage[0], 58*32+1, 2);
            SelectedImage = 0;
        }
        else if((mouseX > StartingX && mouseX < StartingX+IC) && (mouseY > (StartingY+IC) && mouseY < StartingY+(IC * 2)))
        {
            dc.DrawBitmap(ImageStorage[1], 58*32+1, 2);
            SelectedImage = 1;
        }
        else if((mouseX > StartingX && mouseX < StartingX+IC) && (mouseY > (StartingY+(IC*2)) && mouseY < StartingY+(IC * 3)))
        {
            dc.DrawBitmap(ImageStorage[2], 58*32+1, 2);
            SelectedImage = 2;
        }
        else if((mouseX > StartingX && mouseX < StartingX+IC) && (mouseY > (StartingY+(IC*3)) && mouseY < StartingY+(IC * 4)))
        {
            dc.DrawBitmap(ImageStorage[3], 58*32+1, 2);
            SelectedImage = 3;
        }
        else if((mouseX > StartingX && mouseX < StartingX+IC) && (mouseY > (StartingY+(IC*4)) && mouseY < StartingY+(IC * 5)))
        {
            dc.DrawBitmap(ImageStorage[4], 58*32+1, 2);
            SelectedImage = 4;
        }
        else if((mouseX > StartingX && mouseX < StartingX+IC) && (mouseY > (StartingY+(IC*5)) && mouseY < StartingY+(IC * 6)))
        {
            dc.DrawBitmap(ImageStorage[5], 58*32+1, 2);
            SelectedImage = 5;
        }
        else if((mouseX > StartingX && mouseX < StartingX+IC) && (mouseY > (StartingY+(IC*6)) && mouseY < StartingY+(IC * 7)))
        {
            dc.DrawBitmap(ImageStorage[6], 58*32+1, 2);
            SelectedImage = 6;
        }
    }

}

void GameEngineFrame::OnButton8Click(wxCommandEvent& event)
{
    wxString FittingX = "";
    wxString FittingY = "";

    FittingX << NumberOfFittingCellsX;
    FittingY << NumberOfFittingCellsY;

    wxMessageBox(FittingX + " " + FittingY);
}

void GameEngineFrame::OnPanel1Resize(wxSizeEvent& event)
{
    WindowSize = GetClientSize();

    RemainderX = WindowSize.GetWidth() % 64;
    RemainderY = WindowSize.GetHeight() % 64;

    NumberOfFittingCellsX = (WindowSize.GetWidth() - RemainderX) / 64;
    NumberOfFittingCellsY = (WindowSize.GetHeight() - RemainderY) / 64;

    LevelOne.SetUpperX(NumberOfFittingCellsX);
    LevelOne.SetUpperY(NumberOfFittingCellsY);
}

void GameEngineFrame::DrawEditor()
{
     wxClientDC dc(Panel1);

    for(int x=LevelOne.GetLowerX(); x<LevelOne.GetUpperX(); x++)
    {
        for(int y=LevelOne.GetLowerY(); y<LevelOne.GetUpperY(); y++)

        {
            //if((x + y) % 2 == 0)
            //{
                //dc.DrawRectangle(x*32, y*32, 32, 32);
                switch(GridEditor[x][y].GetGraphicID())
                {
                case 0:
                    dc.DrawBitmap(ImageStorage[0], ((x-LevelOne.GetLowerX())*64), ((y-LevelOne.GetLowerY())*64));
                    break;
                case 1:
                    dc.DrawBitmap(ImageStorage[1], ((x-LevelOne.GetLowerX())*64), ((y-LevelOne.GetLowerY())*64));
                    break;
                case 2:
                    dc.DrawBitmap(ImageStorage[2], ((x-LevelOne.GetLowerX())*64), ((y-LevelOne.GetLowerY())*64));
                    break;
                case 3:
                    dc.DrawBitmap(ImageStorage[3], ((x-LevelOne.GetLowerX())*64), ((y-LevelOne.GetLowerY())*64));
                    break;
                case 4:
                    dc.DrawBitmap(ImageStorage[4], ((x-LevelOne.GetLowerX())*64), ((y-LevelOne.GetLowerY())*64));
                    break;
                case 5:
                    dc.DrawBitmap(ImageStorage[5], ((x-LevelOne.GetLowerX())*64), ((y-LevelOne.GetLowerY())*64));
                    break;
                case 6:
                    dc.DrawBitmap(ImageStorage[6], ((x-LevelOne.GetLowerX())*64), ((y-LevelOne.GetLowerY())*64));
                    break;
                default:
                    dc.DrawBitmap(ImageStorage[0], ((x-LevelOne.GetLowerX())*64), ((y-LevelOne.GetLowerY())*64));
                    break;
                }

                /*wxPoint NewPoint;
                NewPoint.x = (x*32)+8;
                NewPoint.y = (y*32)+8;

                dc.DrawText(Grid[x][y].LabelName, NewPoint);*/
            //}

        }
    }

    dc.SetPen(*wxGREY_PEN);
    dc.DrawRectangle((LevelOne.GetMaxX()*64), 0, 318 ,896);
    dc.DrawRectangle(0, (LevelOne.GetMaxY()*64), /*1600*/800 ,69);

    int BitYPos = 64;

    dc.DrawBitmap(ImageStorage[0], (50*32)+1, 1);
    dc.DrawBitmap(ImageStorage[1], (50*32)+1, BitYPos+1);
    dc.DrawBitmap(ImageStorage[2], (50*32)+1, (BitYPos*2)+1);
    dc.DrawBitmap(ImageStorage[3], (50*32)+1, (BitYPos*3)+1);
    dc.DrawBitmap(ImageStorage[4], (50*32)+1, (BitYPos*4)+1);
    dc.DrawBitmap(ImageStorage[5], (50*32)+1, (BitYPos*5)+1);
    dc.DrawBitmap(ImageStorage[6], (50*32)+1, (BitYPos*6)+1);

    dc.SetPen(*wxBLACK_PEN);

    dc.DrawRectangle(58*64, 1, 34, 34);

}


void GameEngineFrame::OnButton6Click(wxCommandEvent& event)//Editor.
{
    InEditor = true;
    MainMenu = false;
    for(unsigned int x=0; x<MainButtons.size(); x++)
    {
        MainButtons[x]->Hide();
    }
    ///You need to completely finish the editor. allow the user to save changes so that the map can be used in game. Then after this is done,you need to set up the compatibility with traverssing through
    ///objects that cannot be walked through.
    if(!LevelCreated)
    {
        CreateLevel();
    }

    DrawEditor();

}

void GameEngineFrame::OnPanel1RightDown(wxMouseEvent& event)
{
    if(InEditor)
    {

        wxClientDC dc(Panel1);

        const wxPoint pt = wxGetMousePosition();

        int mouseX = pt.x; // - this->GetScreenPosition().x;
        int mouseY = pt.y + this->GetScreenPosition().y + 12;



        //wxMessageBox(wxString::Format("%i",(int)mouseX) + " " + wxString::Format("%i",(int)mouseY));

        int StartingX = 1601;
        int StartingY = 52;

        int IC = 64; //Increment value

        int MouseSnapX = mouseX - (mouseX%64);
        int MouseSnapY = (mouseY - (mouseY%64) - 64);

        /*
        MouseX = 36
        /32 = 1 r 6

        MouseX - r = 32;

        36


        */

        switch(SelectedImage)
        {
        case 0:
            dc.DrawBitmap(ImageStorage[0], MouseSnapX, MouseSnapY);
            GridEditor[(MouseSnapX/IC)+LevelOne.GetLowerX()][(MouseSnapY/IC)+LevelOne.GetLowerY()].SetGraphicID(0);
            break;
        case 1:
            dc.DrawBitmap(ImageStorage[1], MouseSnapX, MouseSnapY);
            GridEditor[(MouseSnapX/IC)+LevelOne.GetLowerX()][(MouseSnapY/IC)+LevelOne.GetLowerY()].SetGraphicID(1);
            break;
        case 2:
            dc.DrawBitmap(ImageStorage[2], MouseSnapX, MouseSnapY);
            GridEditor[(MouseSnapX/IC)+LevelOne.GetLowerX()][(MouseSnapY/IC)+LevelOne.GetLowerY()].SetGraphicID(2);
            break;
        case 3:
            dc.DrawBitmap(ImageStorage[3], MouseSnapX, MouseSnapY);
            GridEditor[(MouseSnapX/IC)+LevelOne.GetLowerX()][(MouseSnapY/IC)+LevelOne.GetLowerY()].SetGraphicID(3);
            break;
        case 4:
            dc.DrawBitmap(ImageStorage[4], MouseSnapX, MouseSnapY);
            GridEditor[(MouseSnapX/IC)+LevelOne.GetLowerX()][(MouseSnapY/IC)+LevelOne.GetLowerY()].SetGraphicID(4);
            break;
        case 5:
            dc.DrawBitmap(ImageStorage[5], MouseSnapX, MouseSnapY);
            GridEditor[(MouseSnapX/IC)+LevelOne.GetLowerX()][(MouseSnapY/IC)+LevelOne.GetLowerY()].SetGraphicID(5);
            break;
        case 6:
            dc.DrawBitmap(ImageStorage[6], MouseSnapX-LevelOne.GetLowerX(), MouseSnapY-LevelOne.GetLowerY());
            GridEditor[(MouseSnapX/IC)+LevelOne.GetLowerX()][(MouseSnapY/IC)+LevelOne.GetLowerY()].SetGraphicID(6);
            break;
        default:
            break;
        }

    }
}
