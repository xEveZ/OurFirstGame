#ifndef _MENU_H_

    #define _MENU_H_

    #include "Includer.h"
    #include "main.h"

    #define menu_max_options 10


    class Menu{
        private:
            int menu_index;
            int menu_effettive_n_options;
            pegaAnimation* menu_cursor;
            pegaTextureResource* menu_option[menu_max_options];
            pegaAnimation* menu_caption[menu_max_options];
            int last_texture;
            float x,y;
            float v_spacing;
            float h_spacing;
        public:
            Menu(int startIndexAt,int nOptions, float vSpacing,float hSpacing);
            void setPosx(int posx);
            void setPosy(int posy);
            void draw();
            void setCursorTexture(pegaAnimation* animation);
            void setCursorPosition(int index);
            void previousMenuPosition();
            void nextMenuPosition();
            void addMenuCaption(std::string text);
            void changeMenuCaption(std::string text,int index_pos);
            void selectOption();
            int getCursorPosition();

            ~Menu();
    };


#endif // _MENU_H_
