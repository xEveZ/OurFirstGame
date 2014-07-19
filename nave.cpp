#include "nave.h"

nave::nave(int width,int height,int healthPoints,int ammo_val,float speed_val)
{
    this->w = width;
    this->h = height;
    this->hp = healthPoints;
    this->ammo = ammo_val;
    this->speed = speed_val*0.06;
    this->curr_shot = 0;

    for(int i=0;i<this->ammo;i++)
        this->shots[i] = new colpo(500);


    this->n_animazioni = 0;
    this->alive = true;
    this->_turbo = false;
}

void nave::setPosx(float pos_x)
{
    this->x = pos_x;

}
void nave::setPosy(float pos_y)
{
    this->y = pos_y;

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
    timer[0].reset();
    for(int i=0;i<this->n_animazioni;i++)
    {
        this->animation[i]->setHeight(50);
    }

    if(animToDraw<animazioni_nave)
    {
        this->animation[animToDraw]->setPosition(this->x,this->y);
        this->animation[animToDraw]->draw();
    }
}
void nave::moveBoat(std::string direction)
{
    if(direction=="left")
    {
        this->x-=float(timer[0].getMilliSeconds())*speed;
    }
    else if(direction=="right")
    {
        this->x+=float(timer[0].getMilliSeconds())*speed;
    }
    else if(direction=="up")
    {
        this->y-=float(timer[0].getMilliSeconds())*speed;
    }
    else if(direction=="down")
    {
        this->y+=float(timer[0].getMilliSeconds())*speed;
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

void nave::shotFromHere()
{

    if(timer[1].getMilliSeconds()>200)
    {
        if(this->curr_shot>=this->ammo)
        {
            this->curr_shot = 0;
        }

        this->shots[this->curr_shot]->active = true;
        this->shots[this->curr_shot]->shotFrom(this->x+50,this->y+25);

        this->curr_shot++;
        this->timer[1].reset();
    }
}
bool nave::checkShottableShots(int val)
{
    return this->shots[val]->active;
}

void nave::shot(int param)
{
    //std::cout<<this->shots[param]->getX()<<":"<<this->shots[param]->getY()<<std::endl;
    this->shots[param]->active=this->shots[param]->shot(800,600);
}
int nave::getAmmo()
{
    return this->ammo;
}
