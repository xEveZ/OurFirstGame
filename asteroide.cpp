#include "asteroide.h"

void asteroide::setSpeed(float ast_speed)
{
	this->speed = ast_speed;
}

void asteroide::setPosX(float posx)
{
	this->x =posx;
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
	(void)kind_of_move;
	//non so cosa debba fare la funzione
}
