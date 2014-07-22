#ifndef _COLPO_H_

    #define _COLPO_H_

    #include "Includer.h"
     #include "global.h"

    class colpo
    {
    public:
        colpo(float prj_speed);
        ~colpo();
        float getX();
        float getY();
        void setX(float val);
        void setY(float val);
        void shotFrom(float posx,float posy);
        bool shot(int screenw,int screenh,std::string direction="right");
        void activate(bool val);
        bool isActive();
        void setCenterX(float val);
        float getCenterX();
        void setCenterY(float val);
        float getCenterY();
        void setRadius(float val);
        float getRadius();
        void setActive(bool val);
        void draw();
        bool active;

        circlebox hitbox;
    protected:
        float x,y;
        float speed;
        pegaTextureResource* text_colpo;
        pegaAnimation* anim_colpo;


        pegaTimer timer;

    private:
        //nothing
    };



#endif // _COLPO_H_
