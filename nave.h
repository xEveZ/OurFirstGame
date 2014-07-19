#ifndef _NAVE_H_

    #define _NAVE_H_

    #define animazioni_nave 3

    #define AMMO_NAVE_MAX_SHOTS 100


    #include "Includer.h"
    #include "colpo.h"
    #include "global.h"

    class nave
    {
        private:
            //nothing
        protected:            int w,h;
            float x,y;
            int ammo;
            int curr_shot;
            float speed;
            int hp;
            bool alive;
            bool _turbo;
            double scale;

            colpo* shots[AMMO_NAVE_MAX_SHOTS];
            pegaAnimation* animation[animazioni_nave];
            pegaTimer timer[2];
            int n_animazioni;
        public:
            nave(int width,int height,int healthPoints,int ammo_val,float speed_val);
            void setPosx(float pos_x);
            void setPosy(float pos_y);
            void setSpeed(float speed_val);
            void setAnimation(pegaAnimation* animazione);

            void draw(int animToDraw);
            void setAnimHeight(int which,double h);
            void die();
            void turbo(bool on);
            void shot(int param);
            void shotFromHere();
            void moveBoat(std::string direction);
            bool checkShottableShots(int val);
            void setScale(double scale_value);

            float getSpeed();
            float getPosx();
            float getPosy();
            int getAmmo();

            ~nave();
    };



#endif
