#include "Player.h"

Player::Player(wxString val)
{
    SetName(val);
    SetHealth(100);
    SetGraphicID(3);
    //ctor
}

Player::~Player()
{
    //dtor
}
