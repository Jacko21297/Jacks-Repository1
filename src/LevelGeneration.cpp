#include "LevelGeneration.h"

extern GameEngineFrame* FunctionToSendPtrToFrame();

bool EndCreation = false;
bool WasJumping = false;
int MaxLength = 400;
int ScrollSpeed = 512;
bool FlippedSwitch = false;
Bullet ABullet;

std::vector<std::vector<Block>> Grid;

GameEngineFrame* MyMainFrame;

void GameEngineFrame::InitiateNewGame()
{
    MyMainFrame = FunctionToSendPtrToFrame();
    MainMenu = false;
    WithinGameLoadingScreen = true;
    LoadIntro();
    std::thread GameLoopThread(InitiatingMainThread);
    GameLoopThread.detach();

}

void GameEngineFrame::LoadIntro()
{
    for(unsigned int x=0; x<MainButtons.size(); x++)
    {
        MainButtons[x]->Hide();
    }

    wxPoint IntroPoint;
    IntroPoint.x = WindowSize.GetWidth()/2 - (IntroText->GetSize().GetWidth()/2);
    IntroPoint.y = WindowSize.GetHeight()/2-25;
    IntroText->SetPosition(IntroPoint);

    IntroText->Show();
    wxClientDC dc(Panel1);
    dc.SetBackground(*wxBLACK_BRUSH);
    dc.Clear();

}

void InitiatingMainThread()
{
    while(WithinGameLoadingScreen)//Waits for user to left click
    {

    }
    WithinGame = true;
    MyMainFrame->MainGameLoop();
}

int Tick = 0;

void GameEngineFrame::MainGameLoop()
{
    IntroText->Hide();
    CalledPerTick();
    int Counter = 0;
    int Counter2 = 0;
    int Counter3 = 0;

    wxString TickAsString = "";

    InTopDown = true;
    GamePaused = false;

    while(!GameOver)
    {
        while(InTopDown && !GamePaused)
        {

            Counter++;
            if(Counter > 512)
            {
                Counter = 0;
                Counter2++;
            }
            if(Counter2 > 512)
            {
                Counter2 = 0;
                Counter3++;
            }
            if(Counter3 > 128) //This Function Occurs Every Tick
            {
                Counter3 = 0;
                Tick++;
                TickAsString = "";
                TickAsString << Tick;
                TickCounterLabel->SetLabel(TickAsString);
                CalledPerTick();
            }
            if(Tick > 1024) //This Mainly just stores ticks, so anything after this could be used to initiate multi tick actions.
            {
                Tick = 0;
            }

        }

        Tick = 0;

        if(FirstSwitch)
        {
            FirstSwitch = false;
            GamePaused = false;

            Me.SetPPosX(7);
            Me.SetPPosY(13);

            Me.SetPScreenX(7);
            Me.SetPScreenY(13);

            Me.SetDirection(0);

            TargetOne.SetAPosX(11);
            TargetOne.SetAPosY(13);

            TargetOne.SetAScreenX(11);
            TargetOne.SetAScreenY(13);

            TargetOne.SetAGraphic(3);

            ///You need to draw sprite for target AI, also need to draw some animations :)

            ///You also need to do the boss finish scene where he turns around and shoots you, which is the point at which you are required to jump so you dont die :P

        }

        Tick = 0;

        while(InSideScroller && !GamePaused)
        {

            Counter++;
            if(Counter > 512)
            {
                Counter = 0;
                Counter2++;
            }
            if(Counter2 > ScrollSpeed)
            {
                Counter2 = 0;
                Counter3++;
            }
            if(Counter3 > 128) //This Function Occurs Every Tick
            {
                Counter3 = 0;
                Tick++;
                TickAsString = "";
                TickAsString << Tick;
                TickCounterLabel->SetLabel(TickAsString);
                SCalledPerTick();
            }
            if(Tick > MaxLength) //This Mainly just stores ticks, so anything after this could be used to initiate multi tick actions.
            {
                Tick = 0;
                EndCreation = true;
            }

        }

        Tick = 0;

    }
    Tick = 0;

}

void GameEngineFrame::CloseGate()
{

}

void GameEngineFrame::SCalledPerTick()
{

    Grid = SideScroller.GetMapGrid();

    //if(Me.GetPPosX() + 20 == MaxLength) ScrollSpeed = 1024;
    int MyX, MyY;
    MyX = Me.GetPPosX();
    MyY = Me.GetPPosY();

    if(TargetOne.GetAPosX() > 30)
    {
        TargetOne.SetAGraphic(0);
    }
    if(Me.GetPosX() > SideScroller.GetMaxX())
    {
        GamePaused = true;
        GameOver = true;
    }
    ///Make sure to clear the events stack, might just make a different event stack tbh..
    ///Make sure that you also change the player location at the begginning of the switch to ensure they start at the right place.
    if(!EndCreation)
    {
        SCreateLevel();
    }
    SProcessEvents();

    if((Grid[Me.GetPPosX()+1][Me.GetPPosY()].GetIsPassable() && Me.GetPPosX() < SideScroller.GetMaxX()-2) && !FlippedSwitch)
    {
        SAutoMove();
    }

    if(TargetOne.GetAPosX() >= SideScroller.GetMaxX()) GamePaused = true; GameOver = true;
    if(AutoAIMove())
    {
        TargetOne.SetADirection(2);
    }

    if(SEventStack.size()>1)
    {
        SEventStack.clear();
    }
    if(Grid[Me.GetPPosX()][Me.GetPPosY()].GetGraphicID() == 5)
    {
        Grid[SideScroller.GetMaxX()-11][13].SetGraphicID(3);
        Grid[SideScroller.GetMaxX()-11][13].SetIsPassable(false);
        Grid[SideScroller.GetMaxX()-11][12].SetGraphicID(3);
        Grid[SideScroller.GetMaxX()-11][12].SetIsPassable(false);
        Grid[SideScroller.GetMaxX()-11][11].SetGraphicID(3);
        Grid[SideScroller.GetMaxX()-11][11].SetIsPassable(false);
        Grid[MyX][MyY].SetGraphicID(4);

        SideScroller.SetMapGrid(Grid);
        ScrollSpeed = 2048;

        FlippedSwitch = true;
    }
    if(MyX + 2 >= SideScroller.GetMaxX() && FlippedSwitch != true)
    {
        ///Game Over Screen.
        return;
    }
    SDrawGame();
}

int PrevY = 14;
int TimesCalled = 0;
bool NextY = false;
int AnimationCounter = 0;
bool CreatedBullet = false;

bool GameEngineFrame::AutoAIMove()
{
    Grid = SideScroller.GetMapGrid();


    if(Grid[TargetOne.GetAPosX()+1][TargetOne.GetAPosY()].GetGraphicID() != 3)
    {

        if(Grid[TargetOne.GetAPosX()+1][TargetOne.GetAPosY()].GetIsPassable() == true)
        {
            TargetOne.SetAPosX(TargetOne.GetAPosX()+1);
        }

        if(Grid[TargetOne.GetAPosX()][TargetOne.GetAPosY()+1].GetIsPassable() == true)
        {
            TargetOne.SetAPosY(TargetOne.GetAPosY()+1);
        }
        else if(Grid[TargetOne.GetAPosX()][TargetOne.GetAPosY()-1].GetIsPassable() == true)//SimulateJump
        {
            //TargetOne.SetAPosX(TargetOne.GetAPosX()+1);
            TargetOne.SetAPosY(TargetOne.GetAPosY()-1);
        }


    }
    else
    {
        TargetOne.SetAGraphic(AnimationCounter);
        AnimationCounter++;
        if(AnimationCounter > 5) AnimationCounter = 1;
        if(AnimationCounter == 5)if(!CreatedBullet)CreateBullet();
        ABullet.SetCurrentX(ABullet.GetCurrentX()-1);
        if(Grid[TargetOne.GetAPosX()][TargetOne.GetAPosY()+1].GetIsPassable() == true)
        {
            TargetOne.SetAPosY(TargetOne.GetAPosY()+1);
        }

        return true;


    }

    return false;


    ///



    ///


}

void GameEngineFrame::CreateBullet()
{
    ABullet.SetCurrentX(TargetOne.GetAPosX());

    ABullet.SetCurrentY(TargetOne.GetAPosY());
    ABullet.SetGraphicID(0);
    ABullet.SetDamage(35);
}

void GameEngineFrame::SCreateLevel()
{
    std::vector<Block> RowToInsert;
    Block GrassChecker;

    Grid = SideScroller.GetMapGrid();

    bool IsAtEnd;
    if(SideScroller.GetMaxX() + 20 > MaxLength)
    {
        IsAtEnd = true;
    }
    else
    {
        IsAtEnd = false;
    }

    for(int y=0; y<SideScroller.GetMaxY(); y++){
        Block BlockToPush;
        BlockToPush.SetGraphicID(0);
        BlockToPush.SetHasPhysics(false);
        BlockToPush.SetIsPassable(true);
        BlockToPush.SetStringID("0 ");
        BlockToPush.SetPosX(SideScroller.GetMaxX()+1);
        BlockToPush.SetPosY(y);

        int GeneratedNum = rand()%4+1;

        if(IsAtEnd) GeneratedNum = 5;

            switch(GeneratedNum)
            {
            case 1:
                if(y == PrevY)
                {
                BlockToPush.SetGraphicID(1);
                BlockToPush.SetIsPassable(false);
                PrevY = y;
                }
                break;
            case 2:
                if(y == PrevY-1 && TimesCalled < 1)
                {
                    TimesCalled++;
                    BlockToPush.SetGraphicID(1);
                    BlockToPush.SetIsPassable(false);
                    PrevY = y;
                }
                else if(y == PrevY)
                {
                    BlockToPush.SetGraphicID(1);
                    BlockToPush.SetIsPassable(false);
                    PrevY = y;
                    TimesCalled = 0;
                }
                else
                {
                    TimesCalled = 0;
                }
                break;
            case 5:
                if(y == 14)
                {
                    BlockToPush.SetGraphicID(1);
                    BlockToPush.SetIsPassable(false);
                }
                break;
            case 3:
                if(y == PrevY+1 && PrevY < SideScroller.GetMaxY())
                {
                    BlockToPush.SetGraphicID(1);
                    BlockToPush.SetIsPassable(false);
                    PrevY = y;
                }
            default:
                if(y == PrevY)
                {
                BlockToPush.SetGraphicID(1);
                BlockToPush.SetIsPassable(false);
                PrevY = y;
                }
                break;
            }
                if(y < PrevY && y != 0)
                {
                    BlockToPush.SetGraphicID(0);
                    BlockToPush.SetIsPassable(true);
                    RowToInsert.pop_back();
                    RowToInsert.push_back(BlockToPush);
                }
                if(y > 0)
                {
                    GrassChecker = RowToInsert.back();

                    if(GrassChecker.GetGraphicID() >= 1)
                    {
                        BlockToPush.SetGraphicID(2);
                        BlockToPush.SetIsPassable(false);
                    }
                }

                if(y == 13 && BlockToPush.GetPosX() == MaxLength + 3)
                {
                    BlockToPush.SetGraphicID(5);
                    BlockToPush.SetIsPassable(true);
                }
                else if(y==14 && BlockToPush.GetPosX() == MaxLength + 3)
                {
                    BlockToPush.SetGraphicID(1);
                    BlockToPush.SetIsPassable(false);
                }

                RowToInsert.push_back(BlockToPush);

        }



        Grid.push_back(RowToInsert);

        SideScroller.SetMapGrid(Grid);

        SideScroller.SetMaxX(SideScroller.GetMaxX()+1);


        }

void GameEngineFrame::SAutoMove()
{

    Me.SetDirection(1);

    Grid = SideScroller.GetMapGrid();

    if(WasJumping)
    {
        if(Grid[Me.GetPPosX()][Me.GetPPosY()-1].GetIsPassable())
            {
                Me.SetPPosY(Me.GetPPosY()-1);
                Me.SetPScreenY(Me.GetPScreenY()-1);
            }
            WasJumping = false;
    }

    if(Me.GetPPosX() + 20 > SideScroller.GetUpperX())
        {
            if(SideScroller.GetUpperX() < SideScroller.GetMaxX())
            {
                if(Grid[Me.GetPPosX()+1][Me.GetPosY()].GetIsPassable())
                {
                    SideScroller.SetLowerX(SideScroller.GetLowerX()+1);
                    SideScroller.SetUpperX(SideScroller.GetUpperX()+1);
                    Me.SetPPosX(Me.GetPPosX()+1);
                }
                else
                {
                    return;
                }

            }
            else if(Me.GetPPosX() < SideScroller.GetUpperX()-1)
            {
                if(Grid[Me.GetPPosX()+1][Me.GetPosY()].GetIsPassable())
                {
                    Me.SetPPosX(Me.GetPPosX()+1);
                    Me.SetPScreenX(Me.GetPScreenX()+1);
                }
                else
                {
                    return;
                }
            }
        }
        else if(Me.GetPPosX() < SideScroller.GetUpperX())
        {
            if(Grid[Me.GetPPosX()+1][Me.GetPosY()].GetIsPassable())
            {
                Me.SetPPosX(Me.GetPPosX()+1);
                Me.SetPScreenX(Me.GetPScreenX()+1);
            }
            else
            {
                return;
            }
        }
}

void GameEngineFrame::SProcessEvents()
{

    Grid = SideScroller.GetMapGrid();

    if(SEventStack.size()>0)
    {
        int EventCode = SEventStack.back();
        SEventStack.pop_back();

        switch(EventCode)
        {

            //Put The E' key in
        /*case 1:// Right;
            Me.SetDirection(1);

            if(Me.GetPPosX() + 5 > SideScroller.GetUpperX())
            {
                if(SideScroller.GetUpperX() < SideScroller.GetMaxX())
                {
                    if(Grid[Me.GetPPosX()+1][Me.GetPosY()].GetIsPassable())
                    {
                        SideScroller.SetLowerX(SideScroller.GetLowerX()+1);
                        SideScroller.SetUpperX(SideScroller.GetUpperX()+1);
                        Me.SetPPosX(Me.GetPPosX()+1);
                    }

                }
                else if(Me.GetPPosX() < SideScroller.GetUpperX()-1)
                {
                    if(Grid[Me.GetPPosX()+1][Me.GetPosY()].GetIsPassable())
                    {
                        Me.SetPPosX(Me.GetPPosX()+1);
                        Me.SetPScreenX(Me.GetPScreenX()+1);
                    }
                }

            }
            else if(Me.GetPPosX() < SideScroller.GetUpperX())
            {
                if(Grid[Me.GetPPosX()+1][Me.GetPosY()].GetIsPassable())
                {
                    Me.SetPPosX(Me.GetPPosX()+1);
                    Me.SetPScreenX(Me.GetPScreenX()+1);
                }
            }

            ///You need to complete all controls for the side scroller, so pressing right will go right unless an object is next to it :)

            break;
        case 2: //Left;
            Me.SetDirection(2);

            if(Me.GetPPosX() - 5 < SideScroller.GetLowerX())
            {
                if(SideScroller.GetLowerX() > 0)
                {
                    if(Grid[Me.GetPPosX()-1][Me.GetPosY()].GetIsPassable())
                    {
                        SideScroller.SetLowerX(SideScroller.GetLowerX()-1);
                        SideScroller.SetUpperX(SideScroller.GetUpperX()-1);
                        Me.SetPPosX(Me.GetPPosX()-1);
                    }

                }
                else if(Me.GetPPosX() > SideScroller.GetLowerX())
                {
                    if(Grid[Me.GetPPosX()-1][Me.GetPosY()].GetIsPassable())
                    {
                        Me.SetPPosX(Me.GetPPosX()-1);
                        Me.SetPScreenX(Me.GetPScreenX()-1);
                    }
                }

            }
            else if(Me.GetPPosX() > SideScroller.GetLowerX())
            {
                if(Grid[Me.GetPPosX()-1][Me.GetPosY()].GetIsPassable())
                {
                    Me.SetPPosX(Me.GetPPosX()-1);
                    Me.SetPScreenX(Me.GetPScreenX()-1);
                }
            }



            break;*/
        case 3: //Jump
            if(Me.GetJumps() < 2)
            {
                if(Grid[Me.GetPPosX()][Me.GetPPosY()-1].GetIsPassable())
                {
                    Me.SetPPosY(Me.GetPPosY()-1);
                    Me.SetPScreenY(Me.GetPScreenY()-1);
                    Me.SetJumps(Me.GetJumps()+1);
                    WasJumping = true;
                }
            }
            else //If double jumps reached, use gravity instead to go down.
            {
                if(Grid[Me.GetPPosX()][Me.GetPPosY()+1].GetIsPassable())
                {
                    Me.SetPPosY(Me.GetPPosY()+1);
                    Me.SetPScreenY(Me.GetPScreenY()+1);

                }
                else
                {
                    Me.SetJumps(0);
                }
            }
            break;

        default:
            break;

        }
    }
    else
    {
        Me.SetDirection(0);
        if(Grid[Me.GetPPosX()][Me.GetPPosY()+1].GetIsPassable())
        {
            Me.SetPPosY(Me.GetPPosY()+1);
            Me.SetPScreenY(Me.GetPScreenY()+1);
        }
        else
        {
            Me.SetJumps(0);
        }
    }
}

void GameEngineFrame::SDrawGame()
{

        Grid = SideScroller.GetMapGrid(); //Pulls Grid data from level.
        wxClientDC dc(Panel1);
        wxBitmap Background(WindowSize.GetWidth(), WindowSize.GetHeight());
        wxMemoryDC *MDC = new wxMemoryDC; //Create a pointer to a new memory device context
        MDC->SelectObject(Background); //select the empty bitmap
        //MyMemDC.SelectObject(Background);
        MDC->Clear();

        int GraphicToDraw;

        for(int x=SideScroller.GetLowerX(); x<SideScroller.GetUpperX(); x++)
        {
            for(int y=SideScroller.GetLowerY(); y<SideScroller.GetUpperY(); y++)
            {
                GraphicToDraw = Grid[x][y].GetGraphicID();

                ///This needs to be modified so that it correctly draws the appropriate position on screen.

                MDC->DrawBitmap(SImageStorage[GraphicToDraw], ((x-SideScroller.GetLowerX())*64)+IncrementX, ((y-SideScroller.GetLowerY())*64)+IncrementY);
            }
        }

        if((TargetOne.GetAPosX() < SideScroller.GetUpperX() && TargetOne.GetAPosX() > SideScroller.GetLowerX()) && (TargetOne.GetAPosY() < SideScroller.GetUpperY() && TargetOne.GetAPosY() > SideScroller.GetLowerY()))
        {
            MDC->DrawBitmap(SImage[TargetOne.GetAGraphic()], ((TargetOne.GetAPosX()-SideScroller.GetLowerX())*64)+IncrementX, ((TargetOne.GetAPosY()-SideScroller.GetLowerY())*64)+IncrementY);
        }

        MDC->DrawBitmap(SPlayerImage[Me.GetDirection()], (Me.GetPScreenX()*64)+IncrementX, (Me.GetPScreenY()*64)+IncrementY);
        MDC->DrawBitmap(BImage[0], ABullet.GetCurrentX()*64+IncrementX, ABullet.GetCurrentY()*64+IncrementY);


        MDC->SelectObject(wxNullBitmap); //Free memory for MDC

        delete MDC; //Delete MDC

        dc.DrawBitmap(Background, 0 ,0); //Draw the bitmap that was created previously.
}

void GameEngineFrame::CalledPerTick()
{
    HasSelectedAI->SetLabel("No");
    wxString XLabel = "", YLabel = "";

    XLabel << LevelOne.GetLowerY();
    YLabel << LevelOne.GetUpperY();

    PlayerXLabel->SetLabel(XLabel);
    PlayerYLabel->SetLabel(YLabel);

    if(ProcessEvents()) return;

    if(EventStack.size()>1)
    {
        EventStack.clear();
    }
    //Detect user input
    //Calculate physics etc..
    //Process this
    DrawGame();//Then draw graphics to screen
}
int Counter = 0;
bool GameEngineFrame::ProcessEvents()
{
    if(EventStack.size()>0)
    {
        int EventCode = EventStack.back();
        EventStack.pop_back();

        int CurrentX = Me.GetPPosX();
        int CurrentY = Me.GetPPosY();

        switch(EventCode)
        {
        case 1:
            Me.SetDirection(0);
            if(CurrentY - 5 < LevelOne.GetLowerY()) //Is current position within 5 block radius of lower visual bounds.
            {
                if(LevelOne.GetLowerY() > 0) //Is the visual bounds greater than the lowest grid point.
                {
                    LevelOne.SetLowerY(LevelOne.GetLowerY()-1);//Decrease the lower visual bounds by one.
                    LevelOne.SetUpperY(LevelOne.GetUpperY()-1);
                    Me.SetPPosY(CurrentY-1);//decrease player position by one.
                }
                else if(Me.GetPPosY() > LevelOne.GetLowerY())//if not within 5 block radius, move normal without adjusting visual area. this is for the last 5 blocks of map.
                {
                    Me.SetPPosY(CurrentY-1);
                    Me.SetPScreenY(Me.GetPScreenY()-1);
                }

            }
            else if(Me.GetPPosY() >= LevelOne.GetLowerY())//if its not within 5 block radius and not near 5 blocks of map, move normally
            {
                Me.SetPPosY(CurrentY-1);
                Me.SetPScreenY(Me.GetPScreenY()-1);
            }

            /*(else if(CurrentY > LevelOne.GetLowerY())
            {
                Me.SetPPosY(CurrentY-1);
            }*/
            break;
        case 2:
            Me.SetDirection(1);
            if(CurrentY + 5 > LevelOne.GetUpperY())
            {
                if(LevelOne.GetUpperY() < LevelOne.GetMaxY())
                {
                    LevelOne.SetLowerY(LevelOne.GetLowerY()+1);
                    LevelOne.SetUpperY(LevelOne.GetUpperY()+1);
                    Me.SetPPosY(CurrentY+1);

                }
                else if(Me.GetPPosY() < LevelOne.GetUpperY()-1)
                {
                    Me.SetPPosY(CurrentY+1);
                    Me.SetPScreenY(Me.GetPScreenY()+1);
                }

            }
            else if(Me.GetPPosY() < LevelOne.GetUpperY())
            {
                Me.SetPPosY(CurrentY+1);
                Me.SetPScreenY(Me.GetPScreenY()+1);
            }
            break;
        case 3:
            Me.SetDirection(2);
            if(CurrentX - 5 < LevelOne.GetLowerX()) //Is current position within 5 block radius of lower visual bounds.
            {
                if(LevelOne.GetLowerX() > 0) //Is the visual bounds greater than the lowest grid point.
                {
                    LevelOne.SetLowerX(LevelOne.GetLowerX()-1);//Decrease the lower visual bounds by one.
                    LevelOne.SetUpperX(LevelOne.GetUpperX()-1);
                    Me.SetPPosX(CurrentX-1);//decrease player position by one.
                }
                else if(Me.GetPPosX() > LevelOne.GetLowerX())//if not within 5 block radius, move normal without adjusting visual area. this is for the last 5 blocks of map.
                {
                    Me.SetPPosX(CurrentX-1);
                    Me.SetPScreenX(Me.GetPScreenX()-1);
                }

            }
            else if(Me.GetPPosX() >= LevelOne.GetLowerX())//if its not within 5 block radius and not near 5 blocks of map, move normally
            {
                Me.SetPPosX(CurrentX-1);
                Me.SetPScreenX(Me.GetPScreenX()-1);
            }
            break;
        case 4:
            Me.SetDirection(3);
            if(CurrentX + 5 > LevelOne.GetUpperX())
            {
                if(LevelOne.GetUpperX() < LevelOne.GetMaxX())
                {
                    LevelOne.SetLowerX(LevelOne.GetLowerX()+1);
                    LevelOne.SetUpperX(LevelOne.GetUpperX()+1);
                    Me.SetPPosX(CurrentX+1);

                }
                else if(Me.GetPPosX() < LevelOne.GetUpperX()-1)
                {
                    Me.SetPPosX(CurrentX+1);
                    Me.SetPScreenX(Me.GetPScreenX()+1);
                }

            }
            else if(Me.GetPPosX() < LevelOne.GetUpperX())
            {
                Me.SetPPosX(CurrentX+1);
                Me.SetPScreenX(Me.GetPScreenX()+1);
            }
            break;
        case 5:///This check is basically to check if user is adjacent to the target. this will transfer over to the sidescroller version.
            switch(Me.GetDirection())//This checks to see what direction the user is currently facing to ensure that the user is correctly hitting the AI.
            {
            case 0: //Up
                if(Me.GetPPosX() == TargetOne.GetAPosX() && Me.GetPPosY() - 1 == TargetOne.GetAPosY())
                {
                    //Perform Attempted Kill Animation, perhaps a fireball?
                    //EnterSideScroller
                    HasSelectedAI->SetLabel("Yes");
                    GamePaused = true;
                    InTopDown = false;
                    InSideScroller = true;
                    FirstSwitch = true;
                    return true;
                }
                break;
            case 1://Down
                if(Me.GetPPosX() == TargetOne.GetAPosX() && Me.GetPPosY() + 1 == TargetOne.GetAPosY())
                {
                    //Perform Attempted Kill Animation, perhaps a fireball?
                    //EnterSideScroller
                    HasSelectedAI->SetLabel("Yes");
                    GamePaused = true;
                    InTopDown = false;
                    InSideScroller = true;
                    FirstSwitch = true;
                    return true;
                }
                break;
            case 2://Left
                if(Me.GetPPosX() -1 == TargetOne.GetAPosX() && Me.GetPPosY() == TargetOne.GetAPosY())
                {
                    //Perform Attempted Kill Animation, perhaps a fireball?
                    //EnterSideScroller
                    HasSelectedAI->SetLabel("Yes");
                    GamePaused = true;
                    InTopDown = false;
                    InSideScroller = true;
                    FirstSwitch = true;
                    return true;
                }
                break;
            case 3://right
                if(Me.GetPPosX() +1 == TargetOne.GetAPosX() && Me.GetPPosY() == TargetOne.GetAPosY())
                {
                    //Perform Attempted Kill Animation, perhaps a fireball?
                    //EnterSideScroller
                    HasSelectedAI->SetLabel("Yes");
                    GamePaused = true;
                    InTopDown = false;
                    InSideScroller = true;
                    FirstSwitch = true;
                    return true;
                }
                break;
            default:
                break;
            }

        default:
            break;
        }
    }

    return false;

}

void GameEngineFrame::DrawGame()
{

        Grid = LevelOne.GetMapGrid(); //Pulls Grid data from level.
        wxClientDC dc(Panel1);
        wxBitmap Background(WindowSize.GetWidth(), WindowSize.GetHeight());
        wxMemoryDC *MDC = new wxMemoryDC; //Create a pointer to a new memory device context
        MDC->SelectObject(Background); //select the empty bitmap
        //MyMemDC.SelectObject(Background);
        MDC->Clear();

        int GraphicToDraw;

        /// 0 0 0 0 0 0 0 0 0 0
        /// 0 0 0 0 0 0 0 0 0 0
        /// 0 0 1 1 1 1 1 0 0 0
        /// 0 0 1 1 1 1 1 0 0 0
        /// 0 0 1 1 1 1 1 0 0 0
        /// 0 0 1 1 1 1 1 0 0 0
        /// 0 0 1 1 1 1 1 0 0 0
        /// 0 0 0 0 0 0 0 0 0 0
        /// 0 0 0 0 0 0 0 0 0 0
        /// 0 0 0 0 0 0 0 0 0 0

        /// LowerX = 2; UpperX = 6;
        /// LowerY = 2; UpperY = 6;

        /// LowerX = 3; UpperX = 7;
        for(int x=LevelOne.GetLowerX(); x<LevelOne.GetUpperX(); x++)
        {
            for(int y=LevelOne.GetLowerY(); y<LevelOne.GetUpperY(); y++)
            {
                GraphicToDraw = Grid[x][y].GetGraphicID();

                ///This needs to be modified so that it correctly draws the appropriate position on screen.

                MDC->DrawBitmap(ImageStorage[GraphicToDraw], ((x-LevelOne.GetLowerX())*64)+IncrementX, ((y-LevelOne.GetLowerY())*64)+IncrementY);
            }
        }

        if((TargetOne.GetAPosX() < LevelOne.GetUpperX() && TargetOne.GetAPosX() > LevelOne.GetLowerX()) && (TargetOne.GetAPosY() < LevelOne.GetUpperY() && TargetOne.GetAPosY() > LevelOne.GetLowerY()))
        {
            MDC->DrawBitmap(AIImage[0], ((TargetOne.GetAPosX()-LevelOne.GetLowerX())*64)+IncrementX, ((TargetOne.GetAPosY()-LevelOne.GetLowerY())*64)+IncrementY);
        }

        MDC->DrawBitmap(PlayerImage[Me.GetDirection()], (Me.GetPScreenX()*64)+IncrementX, (Me.GetPScreenY()*64)+IncrementY);


        MDC->SelectObject(wxNullBitmap); //Free memory for MDC

        delete MDC; //Delete MDC

        dc.DrawBitmap(Background, 0 ,0); //Draw the bitmap that was created previously.
}


