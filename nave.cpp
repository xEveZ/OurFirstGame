#include "nave.h"

nave::nave(int width,int height,int healthPoints,int ammo_val,float speed_val)
{
    this->w = width;
    this->h = height;
    this->hp = healthPoints;
    this->ammo = ammo_val;
    this->speed = speed_val;
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
        this->x-=speed;
    }
    else if(direction=="right")
    {
        this->x+=speed;
    }
    else if(direction=="up")
    {
        this->y-=speed;
    }
    else if(direction=="down")
    {
        this->y+=speed;
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

