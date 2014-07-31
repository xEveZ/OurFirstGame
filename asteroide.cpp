#include "asteroide.h"

void asteroide::setSpeed(float ast_speed)
{
	this->speed = ast_speed;
}

void asteroide::setPosX(float posx)
{
	this->x = posx;
}

void asteroide::setPosY(float posy)
{
	this->y = posy;
}

bool asteroide::isAlive()
{
	this->alive = true;
	return this->alive;
}

void asteroide::die()
{
	this->alive= false;
}

void asteroide::moveAst(int kind_of_move)
{
    switch (kind_of_move)
    {
        case DIAGONAL_45_UP_TO_DOWN_RIGHT_SIDE:
            this->setPosX(this->x-(this->speed*this->timer.getMilliSeconds()));
            this->setPosY(this->y+(this->speed*this->timer.getMilliSeconds()));
            this->draw();
            this->timer.reset();
        break;

        case DIAGONAL_45_DOWN_TO_UP_RIGHT_SIDE:
            this->setPosX(this->x-(this->speed*this->timer.getMilliSeconds()));
            this->setPosY(this->y-(this->speed*this->timer.getMilliSeconds()));
            this->draw();
            this->timer.reset();
        break;

        case DIAGONAL_45_UP_TO_DOWN_LEFT_SIDE:
            this->setPosX(this->x+(this->speed*this->timer.getMilliSeconds()));
            this->setPosY(this->y+(this->speed*this->timer.getMilliSeconds()));
            this->draw();
            this->timer.reset();
        break;

        case DIAGONAL_45_DOWN_TO_UP_LEFT_SIDE:
            this->setPosX(this->x+(this->speed*this->timer.getMilliSeconds()));
            this->setPosY(this->y-(this->speed*this->timer.getMilliSeconds()));
            this->draw();
            this->timer.reset();
        break;

        case FORWARD_TO_BACK:
            this->setPosX(this->x-(this->speed*this->timer.getMilliSeconds()));
            this->draw();
            this->timer.reset();
        break;

        case BACK_TO_FORWARD:
            this->setPosX(this->x+(this->speed*this->timer.getMilliSeconds()));
            this->draw();
            this->timer.reset();
        break;

        case DOWN_TO_UP:
            this->setPosY(this->y+(this->speed*this->timer.getMilliSeconds()));
            this->draw();
            this->timer.reset();
        break;

        case UP_TO_DOWN:
            this->setPosY(this->y-(this->speed*this->timer.getMilliSeconds()));
            this->draw();
            this->timer.reset();
        break;
    }

}

void asteroide::draw()
{
    this->anim_asteroide->setPosition(this->x,this->y);
    this->anim_asteroide->draw();
}

asteroide::asteroide()
{
    this->text_asteroide = res->createTextureResource("./img/meteorite.png");
    this->anim_asteroide = animation_manager->createAnimation();
    this->anim_asteroide->pushFrame(this->text_asteroide);
}

asteroide::~asteroide()
{

}
