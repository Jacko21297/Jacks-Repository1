#ifndef BLOCK_H
#define BLOCK_H

#include <Tile.h>


class Block : public Tile
{
    public:
        Block();
        virtual ~Block();

        bool GetIsPassable() { return IsPassable; }
        void SetIsPassable(bool val) { IsPassable = val; }
        bool GetHasPhysics() { return HasPhysics; }
        void SetHasPhysics(bool val) { HasPhysics = val; }
        int GetGraphicID() { return GraphicID; }
        void SetGraphicID(int val) { GraphicID = val; }
        wxString GetStringID() { return StringID; }
        void SetStringID(wxString val) { StringID = val; }

    protected:

    private:
        bool IsPassable;
        bool HasPhysics;
        int GraphicID;
        wxString StringID;
};

#endif // BLOCK_H
