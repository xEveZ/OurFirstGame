#include "nave.h"

nave::nave(int width,int height,int healthPoints,int ammo_val,float speed_val)
{
    this->w = width;
    this->h = height;
    this->hp = healthPoints;
    this->ammo = ammo_val;
    this->speed = speed_val*0.06;
    this->curr_shot = 0;


    if(this->w > this->h)
        this->hitbox.radius = height/2;
    else
        this->hitbox.radius = width/2;

    for(int i=0;i<this->ammo;i++)
        this->shots[i] = new colpo(500);


    this->n_animazioni = 0;
    this->alive = true;
    this->_turbo = false;
}

nave::~nave()
{
    for(int i=0;i<this->n_animazioni;i++)
    {
        animation_manager->deleteAnimation(animation[i]);
    }
}


void nave::setPosx(float pos_x)
{
    this->x = pos_x;
    this->hitbox.center_x = pos_x+this->w/2;

}
void nave::setPosy(float pos_y)
{
    this->y = pos_y;
    this->hitbox.center_y = pos_y+this->h/2;

}
void nave::die()
{
    this->alive = false;
}

void nave::turbo(bool on)
{
    if(on) this->_turbo = true;
    else this->_turbo = false;
}

void nave::setAnimation(pegaAnimation* animazione)
{
    if(this->n_animazioni<animazioni_nave)
    {
        this->animation[this->n_animazioni]=animazione;
        n_animazioni++;
    }
    else
    {
        std::cout<<"ERROR: Numero di animazioni raggiunto"<<std::endl;
    }
}

void nave::draw(int animToDraw)
{
    this->timer[0].reset();
    for(int i=0;i<this->n_animazioni;i++)
    {
        this->animation[i]->setHeight(this->scale);
    }

    if(animToDraw<animazioni_nave)
    {
        this->animation[animToDraw]->setPosition(this->x,this->y);
        this->animation[animToDraw]->draw();
    }
}
void nave::moveBoat(std::string direction,bool bot,float moveto)
{
    if(direction=="left")
    {
        if(!bot)
        {
                this->x-=float(this->timer[0].getMilliSeconds())*speed;
                this->hitbox.center_x = this->x+this->w/2;
        }
        else
        {
            if(this->x>moveto)
            {
                this->x-=float(this->timer[2].getMilliSeconds())*speed;
                this->hitbox.center_x = this->x+this->w/2;
                this->timer[2].reset();

            }

        }
    }
    else if(direction=="right")
    {
        if(!bot)
        {
            this->x+=float(this->timer[0].getMilliSeconds())*speed;
            this->hitbox.center_x = this->x+this->w/2;
        }
        else
        {
            if(this->x<moveto)
            {
                this->x-=float(this->timer[2].getMilliSeconds())*speed;
                this->hitbox.center_x = this->x+this->w/2;
                this->timer[2].reset();

            }
        }
    }
    else if(direction=="up")
    {
        if(!bot)
        {
            this->y-=float(this->timer[0].getMilliSeconds())*speed;
            this->hitbox.center_y = this->y+this->h/2;
        }
        else
        {
            if(this->y>moveto)
            {
                this->y-=float(this->timer[2].getMilliSeconds())*speed;
                this->hitbox.center_y = this->y+this->w/2;
                this->timer[2].reset();

            }
        }
    }
    else if(direction=="down")
    {
        if(!bot)
        {
            this->y+=float(this->timer[0].getMilliSeconds())*speed;
            this->hitbox.center_y = this->y+this->h/2;
        }
        else
        {
            if(this->y<moveto)
            {
                this->y+=float(this->timer[2].getMilliSeconds())*speed;
                this->hitbox.center_y = this->y+this->w/2;
                this->timer[2].reset();

            }
        }
    }
    else
    {
        std::cout<<"specificare una direzione valida (left/right/up/down)"<<std::endl;
    }


}
float nave::getPosx()
{
    return x;
}
float nave::getPosy()
{
    return y;
}

void nave::shotFromHere(std::string direction)
{
    static bool first = true;
    if(first)
    {
        this->timer[2].reset();
        first = false;
    }

    if(this->timer[1].getMilliSeconds()>100)
    {
        if(this->curr_shot>=this->ammo)
        {
            this->curr_shot = 0;
        }

        this->shots[this->curr_shot]->active = true;

        if(direction=="right")
            this->shots[this->curr_shot]->shotFrom(this->x+50,this->y+25);
        else if(direction=="left")
            this->shots[this->curr_shot]->shotFrom(this->x-5,this->y+(this->h/2));

        this->curr_shot++;
        this->timer[1].reset();
    }
}
bool nave::checkShottableShots(int val)
{
    return this->shots[val]->active;
}

void nave::shot(int param,std::string direction)
{
    this->shots[param]->active=this->shots[param]->shot(800,600,direction);
}

int nave::getAmmo()
{
    return this->ammo;
}

void nave::setAnimHeight(int which,double h)
{
        this->animation[which]->setHeight(h);


}

void nave::setScale(double scale_value)
{

    this->scale = scale_value;
}

bool nave::isAlive()
{
    return this->alive;
}
