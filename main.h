#ifndef _MAIN_H_

    #define _MAIN_H_

    #include "Includer.h"
    #include "nave.h"
    #include "global.h"

    std::string convertInt(int number);

    #include "menu.h"
    #include "colpo.h"
    #include "asteroide.h"
    #include "firststage.h"

    extern nave* boat;
    extern bool boat_deleted;            //ogni nave deve avere il proprio bool di controllo per deletare gli oggetti

#endif // _MAIN_H_
