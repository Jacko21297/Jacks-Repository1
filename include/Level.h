#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include "Item.h"
#include "Block.h"

class Level
{
    public:
        Level();
        virtual ~Level();

        wxString GetLevelName() { return LevelName; }
        void SetLevelName(wxString val) { LevelName = val; }
        int GetLevelID() { return LevelID; }
        void SetLevelID(int val) { LevelID = val; }

        int GetMaxX() { return MaxX; }
        void SetMaxX(int val) { MaxX = val; }
        int GetMaxY() { return MaxY; }
        void SetMaxY(int val) { MaxY = val; }

        int GetLowerX() { return LowerX; }
        void SetLowerX(int val) { LowerX = val; }
        int GetLowerY() { return LowerY; }
        void SetLowerY(int val) { LowerY = val; }

        int GetUpperX() { return UpperX; }
        void SetUpperX(int val) { UpperX = val; }
        int GetUpperY() { return UpperY; }
        void SetUpperY(int val) { UpperY = val; }


        int GetSeed() { return Seed; }
        void SetSeed(int val) { Seed = val; }
        std::vector<std::vector<Block>> GetMapGrid() { return MapGrid; }
        void SetMapGrid(std::vector<std::vector<Block>> val) { MapGrid = val; }
        std::vector<std::vector<Item>> GetItemMapGrid() { return ItemMapGrid; }
        void SetItemMapGrid(std::vector<std::vector<Item>> val) { ItemMapGrid = val; }

    protected:

    private:
        wxString LevelName;
        std::vector<std::vector<Block>> MapGrid;
        std::vector<std::vector<Item>> ItemMapGrid;
        int LevelID;
        int MaxX;
        int MaxY;
        int Seed;
        int LowerX;
        int LowerY;
        int UpperX;
        int UpperY;
};

#endif // LEVEL_H
