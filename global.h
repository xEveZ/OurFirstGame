#ifndef _GLOBAL_H_
    #define _GLOBAL_H_


    #define N_NAVI_NEMICHE_MODELLO_1 3


    #include "Includer.h"


    extern pegaAnimationManager* animation_manager;
    extern pegaFontRenderer* font_renderer;
    extern pegaFontResource* font_batman;
    extern pegaResourceManager* res;
    extern pegaAnimation* background_anim;
    extern float backround_x_variable;
    extern pegaTimer timer;


    extern pegaAnimation* animation_enemy[N_NAVI_NEMICHE_MODELLO_1];


    extern int resx;
    extern int resy;
    typedef struct{
        float center_x;
        float center_y;
        float radius;
    }circlebox;


#endif // _GLOBAL_H_
