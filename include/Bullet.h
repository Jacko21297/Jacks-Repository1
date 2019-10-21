#ifndef BULLET_H
#define BULLET_H


class Bullet
{
    public:
        Bullet();
        virtual ~Bullet();

        int GetCurrentX() { return CurrentX; }
        void SetCurrentX(int val) { CurrentX = val; }
        int GetCurrentY() { return CurrentY; }
        void SetCurrentY(int val) { CurrentY = val; }
        bool GetHasHit() { return HasHit; }
        void SetHasHit(bool val) { HasHit = val; }
        int GetDamage() { return Damage; }
        void SetDamage(int val) { Damage = val; }

        int GetGraphicID() { return GraphicID; }
        void SetGraphicID(int val) { GraphicID = val; }

    protected:

    private:
        int CurrentX;
        int CurrentY;
        bool HasHit;
        int Damage;
        int GraphicID;
};

#endif // BULLET_H
