#ifndef LEVELGENERATION_H_INCLUDED
#define LEVELGENERATION_H_INCLUDED

#include "../GameEngineMain.h"
#include <vector>
#include <thread>

#include "Tile.h"
#include "Block.h"
#include "Player.h"
#include "Item.h"
#include "Level.h"
#include "TargetAI.h"
#include "Bullet.h"

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>

void InitiatingMainThread();
void MainGameLoop();

extern std::vector<wxButton*> MainButtons; //stores buttons for main menu.
extern wxSize WindowSize; //Used to store the windows size.
extern bool MainMenu;
extern bool GameOver;
extern bool WithinGameLoadingScreen;
extern std::vector<wxBitmap> ImageStorage; //Stores all images required during applications use.
extern std::vector<wxBitmap> PlayerImage;
extern std::vector<wxBitmap> AIImage;
extern std::vector<int> EventStack; //All events get pushed onto this stack to be dealt with by the Custom Event Handler.
extern std::vector<int> SEventStack;
extern std::vector<wxBitmap> SImageStorage;
extern std::vector<wxBitmap> SPlayerImage;
extern std::vector<wxBitmap> SImage;
extern std::vector<wxBitmap> BImage;

extern Level LevelOne;
extern Level SideScroller;
extern bool WithinGame;
extern bool GamePaused;
extern bool InTopDown;
extern bool InSideScroller;
extern bool FirstSwitch;
extern int IncrementX;
extern int IncrementY;
extern Player Me;
extern TargetAI TargetOne;

#endif // LEVELGENERATION_H_INCLUDED
