#include "colpo.h"

colpo::colpo(float prj_speed)
{
    this->radius = 5;
    this->text_colpo = res->createTextureResource("./img/colpo.png");
    this->anim_colpo = animation_manager->createAnimation();

    this->anim_colpo->pushFrame(this->text_colpo);

    this->speed = prj_speed*0.05;

    this->active = false;

}

colpo::~colpo()
{



}

float colpo::getX()
{
    return this->x;
}
float colpo::getY()
{
    return this->y;
}
void colpo::shotFrom(float posx,float posy)
{
    this->x = posx;
    this->y = posy;

    this->center_x = posx+5;
    this->center_y = posy+5;
}

bool colpo::shot(int screenw,int screenh,std::string direction)
{
    if(direction=="right")
    {
        if(this->x<screenw)
        {
            this->anim_colpo->setPosition(this->x,this->y);
            this->center_x = this->x + 5;
            this->center_y = this->y + 5;
            this->x+=this->speed;
            if(this->active)
                this->anim_colpo->draw();

            this->active = true;
            return true;
        }
        else
        {
            this->active = false;
            return false;
        }
    }
    else if(direction=="left")
    {
        if(this->x>0)
        {
            this->anim_colpo->setPosition(this->x,this->y);
            this->center_x = this->x + 5;
            this->center_y = this->y + 5;
            this->x-=float(this->timer.getMilliSeconds())*this->speed;
            if(this->active)
                this->anim_colpo->draw();

            this->active = true;
            return true;
        }
        else
        {
            this->active = false;
            return false;
        }
    }

}

void colpo::activate(bool val)
{
    this->active = val;
}

bool colpo::isActive()
{
    return this->active;
}
void colpo::setCenterX(float val)
{

    this->center_x = val;

}

float colpo::getCenterX()
{
    return this->center_x;
}

void colpo::setCenterY(float val)
{
    this->center_y = val;
}

float colpo::getCenterY()
{
    return this->center_y;
}

void colpo::setRadius(float val)
{
    this->radius = val;
}

float colpo::getRadius()
{
    return this->radius;
}

void colpo::setActive(bool val)
{
    this->active = val;
}

void colpo::setX(float val)
{
    this->x = val;
}

void colpo::setY(float val)
{
    this->y = val;
}
void colpo::draw()
{
    this->timer.reset();
    this->anim_colpo->setPosition(this->x,this->y);
    this->anim_colpo->draw();

}
