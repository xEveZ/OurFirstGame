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
            float atkSpeed;
            bool _turbo;
            double scale;
            pegaAnimation* animation[animazioni_nave];
            pegaTimer timer[3];
            int n_animazioni;
        public:
            circlebox hitbox;
            colpo* shots[AMMO_NAVE_MAX_SHOTS];
            nave(int width,int height,int healthPoints,int ammo_val,float speed_val,float atk_speed,float prj_speed=500);
            void setPosx(float pos_x);
            void setPosy(float pos_y);
            void setSpeed(float speed_val);
            void setAnimation(pegaAnimation* animazione);

            void draw(int animToDraw);
            void setAnimHeight(int which,double h);
            void die();
            void turbo(bool on);
            void shot(int param,std::string direction);
            void shotFromHere(std::string direction);
            void moveBoat(std::string direction,bool bot=false,float moveto=0);
            void drawHitbox();

            bool checkShottableShots(int val);
            void setScale(double scale_value);
            bool isAlive();
            float getSpeed();
            float getPosx();
            float getPosy();
            int getAmmo();

            ~nave();
    };



#endif
