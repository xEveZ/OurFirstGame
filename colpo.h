#ifndef _COLPO_H_

    #define _COLPO_H_

    #include "Includer.h"
    #include "main.h"

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
    protected:
        float x,y;
        float speed;
        pegaTextureResource* text_colpo;
        pegaAnimation* anim_colpo;
        float center_x,center_y;
        float radius;
        bool active;

    private:
        //nothing
    };



#endif // _COLPO_H_
