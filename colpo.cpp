#include "colpo.h"

colpo::colpo(float prj_speed)
{
    this->hitbox.radius = 4;
    this->text_colpo = res->createTextureResource("./img/colpo.png");
    this->anim_colpo = animation_manager->createAnimation();
    this->anim_colpo->pushFrame(this->text_colpo);

    this->anim_colpo->setHeight(8);

    this->speed = prj_speed*0.001;

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

    this->hitbox.center_x = posx+4;
    this->hitbox.center_y = posy+4;

    this->timer.reset();
}

bool colpo::shot(int screenw,int screenh,std::string direction)
{
    (void)screenh;
    if(direction=="right")
    {
        if(this->x<screenw+20)
        {
            this->anim_colpo->setPosition(this->x,this->y);
            this->hitbox.center_x = this->x + 4;
            this->hitbox.center_y = this->y + 4;
            this->x+=float(this->timer.getMilliSeconds())*this->speed;
            this->timer.reset();
            if(this->active)
                this->anim_colpo->draw();

            this->active = true;
            return true;
        }
        else
        {
            this->active = false;
            this->setX(-10.0);
            this->setY(-10.0);
            return false;
        }

    }
    else if(direction=="left")
    {
        if(this->x>-20)
        {
            this->anim_colpo->setPosition(this->x,this->y);
            this->hitbox.center_x = this->x + 4;
            this->hitbox.center_y = this->y + 4;
            this->x-=float(this->timer.getMilliSeconds())*this->speed;
            this->timer.reset();
            if(this->active)
                this->anim_colpo->draw();

            this->active = true;
            return true;
        }
        else
        {
            this->active = false;
            this->setX(-10.0);
            this->setY(-10.0);
            return false;
        }
    }
    return false;
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

    this->hitbox.center_x = val;

}

float colpo::getCenterX()
{
    return this->hitbox.center_x;
}

void colpo::setCenterY(float val)
{
    this->hitbox.center_y = val;
}

float colpo::getCenterY()
{
    return this->hitbox.center_y;
}

void colpo::setRadius(float val)
{
    this->hitbox.radius = val;
}

float colpo::getRadius()
{
    return this->hitbox.radius;
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
