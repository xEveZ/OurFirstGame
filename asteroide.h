#ifndef _ASTEROIDE_H_

    #define _ASTEROIDE_H_

    #define DIAGONAL_45_UP_TO_DOWN_RIGHT_SIDE 0
    #define DIAGONAL_45_DOWN_TO_UP_RIGHT_SIDE 1
    #define DIAGONAL_45_UP_TO_DOWN_LEFT_SIDE 2
    #define DIAGONAL_45_DOWN_TO_UP_LEFT_SIDE 3
    #define FORWARD_TO_BACK 4
    #define BACK_TO_FORWARD 5
    #define DOWN_TO_UP 6
    #define UP_TO_DOWN 7

    #include "Includer.h"
    #include "global.h"

    class asteroide{

        private:
            float x,y;
            pegaTextureResource* text_asteroide;
            pegaAnimation* anim_asteroide;
            float speed;
            float alive;
        public:
            circlebox hitbox;
            asteroide(float ast_speed);
            ~asteroide();
            void setSpeed(float ast_speed);
            void setPosX(float posx);
            void setPosY(float posy);
            void die();
            bool isAlive();
            void moveAst(int kind_of_move = DIAGONAL_45_UP_TO_DOWN_RIGHT_SIDE);
    };





#endif // _ASTEROIDE_H_
