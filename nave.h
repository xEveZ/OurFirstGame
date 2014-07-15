#ifndef _NAVE_H_

    #define _NAVE_H_

    #define animazioni_nave 3


    #include "Includer.h"
    #include "main.h"

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

            //classe colpo LOL
            pegaAnimation* animation[animazioni_nave];
            int n_animazioni;
        public:
            nave(int width,int height,int healthPoints,int ammo_val,float speed_val);
            void setPosx(float pos_x);
            void setPosy(float pos_y);
            void setSpeed(float speed_val);
            void setAnimation(pegaAnimation* animazione);

            void draw(int animToDraw);
            void die();
            void turbo(bool on);

            void moveBoat(std::string direction);


            float getSpeed();
            float getPosx();
            float getPosy();


            ~nave();
    };



#endif
