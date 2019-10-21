#ifndef PLAYER_H
#define PLAYER_H

#include <Tile.h>

class Player : public Tile
{
    public:
        Player(wxString val);
        virtual ~Player();

        int GetHealth() { return Health; }
        void SetHealth(int val) { Health = val; }
        int GetGraphicID() { return GraphicID; }
        void SetGraphicID(int val) { GraphicID = val; }
        wxString GetName() { return Name; }
        void SetName(wxString val) { Name = val; }

        int GetPPosX() { return PPosX; }
        void SetPPosX(int val) { PPosX = val; }
        int GetPPosY() { return PPosY; }
        void SetPPosY(int val) { PPosY = val; }

        int GetPPrevX() { return PPrevX; }
        void SetPPrevX(int val) { PPrevX = val; }
        int GetPPrevY() { return PPrevY; }
        void SetPPrevY(int val) { PPrevY = val; }

        int GetPScreenX() { return PScreenX; }
        void SetPScreenX(int val) { PScreenX = val; }
        int GetPScreenY() { return PScreenY; }
        void SetPScreenY(int val) { PScreenY = val; }

        int GetDirection() { return Direction; }
        void SetDirection(int val) { Direction = val; }

        int GetJumps() { return Jumps; }
        void SetJumps(int val) { Jumps = val; }


    protected:

    private:
        int Health;
        int GraphicID;
        wxString Name;

        int PScreenX, PScreenY;
        int PPosX, PPrevX;
        int PPosY, PPrevY;

        int Direction;

        int Jumps;

};

#endif // PLAYER_H
