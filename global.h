#ifndef _GLOBAL_H_
    #define _GLOBAL_H_

    #include "Includer.h"

    extern pegaAnimationManager* animation_manager;
    extern pegaFontRenderer* font_renderer;
    extern pegaFontResource* font_batman;
    extern pegaResourceManager* res;

    typedef struct{
        float center_x;
        float center_y;
        float radius;
    }circlebox;


#endif // _GLOBAL_H_
