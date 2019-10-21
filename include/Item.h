#ifndef ITEM_H
#define ITEM_H

#include <Tile.h>


class Item : public Tile
{
    public:
        Item();
        virtual ~Item();

        int GetGraphicID() { return GraphicID; }
        void SetGraphicID(int val) { GraphicID = val; }
        wxString GetStringID() { return StringID; }
        void SetStringID(wxString val) { StringID = val; }
        int GetActionToInitiate() { return ActionToInitiate; }
        void SetActionToInitiate(int val) { ActionToInitiate = val; }
        bool GetHasPhysics() { return HasPhysics; }
        void SetHasPhysics(bool val) { HasPhysics = val; }
        bool GetExists() { return Exists; }
        void SetExists(bool val) { Exists = val; }

    protected:

    private:
        int GraphicID;
        wxString StringID;
        int ActionToInitiate;
        bool HasPhysics;
        bool Exists;
};

#endif // ITEM_H
