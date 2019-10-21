#ifndef TARGETAI_H
#define TARGETAI_H


class TargetAI
{
    public:
        TargetAI();
        virtual ~TargetAI();

        int GetAPosX() { return APosX; }
        void SetAPosX(int val) { APosX = val; }
        int GetAPosY() { return APosY; }
        void SetAPosY(int val) { APosY = val; }
        int GetHealth() { return Health; }
        void SetHealth(int val) { Health = val; }
        wxString GetName() { return Name; }
        void SetName(wxString val) { Name = val; }

        int GetAScreenX() { return AScreenX; }
        void SetAScreenX(int val) { AScreenX = val; }
        int GetAScreenY() { return AScreenY; }
        void SetAScreenY(int val) { AScreenY = val; }

        int GetADirection() { return ADirection; }
        void SetADirection(int val) { ADirection = val; }

        int GetAGraphic() { return AGraphic; }
        void SetAGraphic(int val) { AGraphic = val; }

    protected:

    private:
        int APosX, APosY;
        int AScreenX, AScreenY;
        int Health;
        int ADirection;
        int AGraphic;
        wxString Name;
};

#endif // TARGETAI_H
