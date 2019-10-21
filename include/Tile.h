#ifndef TILE_H
#define TILE_H

class Tile
{
    public:
        Tile();
        virtual ~Tile();

        int GetPosX() { return PosX; }
        void SetPosX(int val) { PosX = val; }
        int GetPosY() { return PosY; }
        void SetPosY(int val) { PosY = val; }

    protected:

    private:
        int PosX;
        int PosY;
};

#endif // TILE_H
