#include "menu.h"


Menu::Menu(int startIndexAt,int nOptions, float vSpacing,float hSpacing)
{
    this->menu_index = startIndexAt;
    this->menu_effettive_n_options = nOptions;
    this->v_spacing = vSpacing;
    this->h_spacing = hSpacing;
    this->last_texture = 0;
}
void Menu::setPosx(int posx)
{
    this->x = posx;
}
void Menu::setPosy(int posy)
{
    this->y = posy;
}
void Menu::setCursorTexture(pegaAnimation* animation)
{
    this->menu_cursor = animation;
    this->menu_cursor->setHeight(35);
}

void Menu::addMenuCaption(std::string text)
{
    if(this->last_texture<this->menu_effettive_n_options)
    {
        this->menu_option[this->last_texture] = font_renderer->generateString(text.c_str());
        this->menu_caption[this->last_texture] = animation_manager->createAnimation();

        this->menu_caption[this->last_texture]->pushFrame(this->menu_option[this->last_texture]);
        this->last_texture++;
    }
}

void Menu::changeMenuCaption(std::string text,int index_pos)
{
        this->menu_option[index_pos] = font_renderer->generateString(text.c_str());
        this->menu_caption[index_pos] = animation_manager->createAnimation();

        this->menu_caption[index_pos]->pushFrame(this->menu_option[index_pos]);
}
void Menu::draw()
{
    for(int i=0;i<this->last_texture;i++)
    {
        this->menu_caption[i]->setPosition(x,y+(i*this->v_spacing));
        this->menu_cursor->setPosition(x-50,y+(this->menu_index*this->v_spacing));
        this->menu_caption[i]->draw();
        this->menu_cursor->draw();
    }
}

void Menu::setCursorPosition(int index)
{
    this->menu_index = index;
    if(this->menu_index<this->menu_effettive_n_options && this->menu_index>=0)
    {
        this->menu_cursor->setPosition(this->x-50,this->y+(this->menu_index*this->v_spacing));
    }
    else
        std::cout<<"ERROR: WTF ARE U DOIN"<<std::endl;
}

void Menu::nextMenuPosition()
{
    if(this->menu_index<this->menu_effettive_n_options-1)
    {
        this->menu_index++;
        this->menu_cursor->setPosition(this->x-50,this->y+(this->menu_index*this->v_spacing));
    }
    else
        std::cout<<"ERROR: valore di menu_index: "<<this->menu_index<<" non valido"<<std::endl;
}

void Menu::previousMenuPosition()
{
    if(this->menu_index>0)
    {
        this->menu_index--;
        this->menu_cursor->setPosition(this->x-50,this->y+(this->menu_index*this->v_spacing));
    }
    else
        std::cout<<"ERROR: valore di menu_index: "<<this->menu_index<<" non valido"<<std::endl;
}

void Menu::selectOption()
{

}

int Menu::getCursorPosition()
{
    return this->menu_index;
}

Menu::~Menu()
{

}
