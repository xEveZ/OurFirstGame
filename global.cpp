#include "global.h"


    pegaAnimationManager* animation_manager;    //gestore delle animazioni
    pegaFontRenderer* font_renderer;
    pegaFontResource* font_batman;
    pegaResourceManager* res;
    pegaAnimation* background_anim;
    pegaAnimation* animation_enemy[N_NAVI_NEMICHE_MODELLO_1];
    float backround_x_variable = 0;
    pegaTimer timer;
    int resx;
    int resy;
