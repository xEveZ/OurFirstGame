#include "main.h"



//{                     VARIABILI GLOBALI AL PROGETTO
    nave* boat; bool boat_deleted=false;            //ogni nave deve avere il proprio bool di controllo per deletare gli oggetti


    std::string convertInt(int number)  //funzione per convertire da intero a string (utilizzata ad esempio per stampare l'hud)
    {
       std::stringstream ss;
       ss << number;
       return ss.str();
    }

    bool collision(circlebox n1, circlebox n2)
    {
        float sum = n1.radius + n2.radius;
        sum*=sum;
        float dist = (n2.center_x-n1.center_x)*(n2.center_x-n1.center_x)+(n2.center_y-n1.center_y)*(n2.center_y-n1.center_y);
        if(dist<=sum)return true;
        else         return false;
    }


//}

//{                     VARIABILI GLOBALE AL DOCUMENTO
    void displayFunction();

    void mainMenu();
    void inGame();
    void OptionMenu();
    void ShutGame();

    bool ingame=false;
    bool main_menu=true;
    bool option_menu=false;
    bool reboot_message = false;
    bool edited = false;


    pegaTextureResource* nave_anim_1[7];                //texture della nave principale


    pegaTextureResource* enemy_anim[7];
    pegaTextureResource* background_text;
    pegaTimer timer2;
    Menu* menu;
    Menu* menu_option;

    bool up=false;                                      //boolean per gestire il movimento della nave con gli eventi keydown/up
    bool down=false;
    bool right=false;
    bool left=false;
    bool z=false;
    std::string resolution[]= {"    < 640x480 >","    < 800x600 >","    < 1024x768 >","    < 1366X768 >"};
    int videochoice=0;

    int resx_bak,resy_bak;
    bool fullscreen=false,sounds=false,fullscreen_bak,sounds_bak;

//}

double calcola_angolo(double x,double y)
{
    if(x==0.0 && y==0.0)
        return 0.0;
    else
    {
       return atan2(y,x);
    }
}
double calcola_magnitude(double x,double y)
{
    return sqrt(pow(x,2)+pow(y,2));
}


int main(int argc,char** argv)
{
    bool quit = false;
    bool changing_vals=false;
    double joy_x,joy_y;
    double magnitude;
    pegaDevice* dev = new pegaDevice(&argc,&argv);

    pegaWindow* window;
    pegaEvent* event;

    pegaAnimation* animation;

    mainMenu();

    std::fstream f;

        f.open("options.txt",std::fstream::in);

        if(f.good())
        {
            std::string app;

            f>>app;

            resx = atoi(app.c_str());
            f>>app;
            resy = atoi(app.c_str());

            f>>app;

            if(app=="ON")
                fullscreen = true;
            else
                fullscreen = false;

            f>>app;

            if(app=="ON")
                sounds = true;
            else
                sounds = false;

            switch(resx)
            {
                case 640:
                    videochoice=0;
                break;
                case 800:
                    videochoice=1;
                break;
                case 1024:
                    videochoice=2;
                break;
                case 1366:
                    videochoice=3;
                break;
            }
        }
        else
        {
            resx = 800;
            resy = 600;
            sounds = true;
            fullscreen = false;
        }
    f.close();

    fullscreen_bak = fullscreen;
    sounds_bak = sounds;
    resx_bak = resx;
    resy_bak = resy_bak;

    window = dev->createWindow("Titolo LOL",PEGA_ANTIALIASING_HIGH,fullscreen,resx,resy);
    window->showCursor(false);
    res = dev->getResourceManager();
    font_renderer = dev->getFontRenderer();
    animation_manager = dev->getAnimationManager();
    font_batman = res->createFontResource("./fonts/batmfa.ttf",30);
    font_renderer->setFontResource(font_batman);

    menu = new Menu(0,3,60,0);
    menu_option = new Menu(0,7,50,0);
    boat = new nave(50,50,100,30,5.0,150);            //più il valore dell'atk_speed è basso e più spara veloce, rappresenta
                                                      //l'intervallo di tempo da aspettare prima di poter sparare un altro colpo


    for(int i=0;i<7;i++)
    {
        std::string app;
        app = "./img/nave[" + convertInt(i) + "].png";
        nave_anim_1[i] = res->createTextureResource(app.c_str());
        app = "./img/enemy1[" + convertInt(i) + "].png";
        enemy_anim[i] = res->createTextureResource(app.c_str());
    }

    background_text = res->createTextureResource("./img/1600x600.png");
    background_anim = animation_manager->createAnimation();
    animation = animation_manager->createAnimation();


    for(int k=0;k<N_NAVI_NEMICHE_MODELLO_1;k++)
        animation_enemy[k] = animation_manager->createAnimation();


    for(int i=0;i<7;i++)
    {
        animation->pushFrame(nave_anim_1[i]);
        for(int k=0;k<N_NAVI_NEMICHE_MODELLO_1;k++)
            animation_enemy[k]->pushFrame(enemy_anim[i]);
    }
    background_anim->pushFrame(background_text);
    animation->setFPS(20);

    for(int k=0;k<N_NAVI_NEMICHE_MODELLO_1;k++)
        animation_enemy[k]->setFPS(20);

    boat->setAnimation(animation);
    boat->setScale(50);
    boat->setPosx(0);
    boat->setPosy(resy/2);

    setlevel_one();

    background_anim->setPosition(0,0);

    menu->setPosx(resx/2);
    menu->setPosy(resy-300);

    menu->addMenuCaption("Play Now");
    menu->addMenuCaption("Options");
    menu->addMenuCaption("Quit");

    menu->setCursorTexture(animation);

    menu_option->setPosx(resx/2);
    menu_option->setPosy(resy-400);

    menu_option->addMenuCaption("RESOLUTION");
    menu_option->addMenuCaption(resolution[videochoice]);
    menu_option->addMenuCaption("FULLSCREEN");

    if(fullscreen)
        menu_option->addMenuCaption("    < ON >");
    else
        menu_option->addMenuCaption("    < OFF >");

    menu_option->addMenuCaption("SOUNDS");

    if(sounds)
        menu_option->addMenuCaption("    < ON >");
    else
        menu_option->addMenuCaption("    < OFF >");

    menu_option->addMenuCaption("BACK");

    menu_option->setCursorTexture(animation);

    while(!quit)
    {
        int index_app; //                           Controlli per Option Menu nel secondo switch dei keydown


        window->beginScene();

            displayFunction();

        window->endScene();
        while((event=window->getEvent()))
        {
            switch(event->type)
            {
                case PEGA_EVENT_QUIT:
                    quit = true;
                break;
                case PEGA_EVENT_JOYDPAD:

                    if(event->joydpad->x==-1) left = true;
                    else left = false;

                    if(event->joydpad->x==1) right = true;
                    else right = false;

                    if(event->joydpad->y==-1) up = true;
                    else up=false;

                    if(event->joydpad->y==1) down = true;
                    else down=false;

                break;
                case PEGA_EVENT_JOYBUTTONDOWN:
                    if(event->joybutton->button==1)
                        z=true;

                break;
                case PEGA_EVENT_JOYBUTTONUP:
                    if(event->joybutton->button==1)
                        z=false;


                break;
                case PEGA_EVENT_JOYAXIS:

                if(ingame)
                {
                    switch(event->joyaxis->axis)
                    {
                        case 0:

                            joy_x = event->joyaxis->value;

                        break;
                        case 1:

                            joy_y = event->joyaxis->value;
                        break;
                        default:

                        break;
                    }
                    boat->addCoord(joy_x,joy_y);
                    std::cout<<boat->getPosx()<<"    "<<boat->getPosy()<<std::endl;

                }

                break;
                case PEGA_EVENT_KEYDOWN:
                    switch(event->key->code)
                    {
                        case PEGA_KEY_Z:

                            z=true;

                        break;
                        case PEGA_KEY_UP:
                        case PEGA_KEY_W:
                            if(main_menu)
                                menu->previousMenuPosition();
                            else if(ingame)
                                up=true;
                            else if(option_menu)
                            {
                                if(!changing_vals)
                                {
                                    index_app = menu_option->getCursorPosition();
                                    menu_option->previousMenuPosition();
                                    menu_option->previousMenuPosition();
                                }
                            }

                        break;
                        case PEGA_KEY_DOWN:
                        case PEGA_KEY_S:
                            if(main_menu)
                                menu->nextMenuPosition();
                            else if(ingame)
                                down=true;
                            else if(option_menu)
                            {
                                if(!changing_vals)
                                {
                                    index_app = menu_option->getCursorPosition();
                                    menu_option->nextMenuPosition();
                                    menu_option->nextMenuPosition();
                                }

                            }
                        break;
                        case PEGA_KEY_LEFT:
                        case PEGA_KEY_A:
                            if(ingame)
                                left=true;
                            else if(option_menu)
                            {
                                switch(menu_option->getCursorPosition())
                                {
                                    case 1:
                                        if(videochoice>0)
                                            videochoice--;

                                        menu_option->changeMenuCaption(resolution[videochoice],1);

                                        switch(videochoice)
                                        {
                                            case 0:
                                                resx = 640;
                                                resy = 480;
                                            break;
                                            case 1:
                                                resx = 800;
                                                resy = 600;
                                            break;
                                            case 2:
                                                resx = 1024;
                                                resy = 768;
                                            break;
                                            case 3:
                                                resx = 1366;
                                                resy = 768;
                                            break;
                                            default:
                                            break;
                                        }




                                    break;
                                    case 3:
                                        fullscreen = false;

                                        menu_option->changeMenuCaption("    < OFF >",3);
                                    break;
                                    case 5:
                                        sounds = false;
                                        menu_option->changeMenuCaption("    < OFF >",5);
                                    break;
                                }
                            }

                        break;
                        case PEGA_KEY_RIGHT:
                        case PEGA_KEY_D:
                            if(ingame)
                                right=true;
                             else if(option_menu)
                            {
                                switch(menu_option->getCursorPosition())
                                {
                                    case 1:
                                        if(videochoice<3)
                                            videochoice++;

                                        menu_option->changeMenuCaption(resolution[videochoice],1);

                                        switch(videochoice)
                                        {
                                            case 0:
                                                resx = 640;
                                                resy = 480;
                                            break;
                                            case 1:
                                                resx = 800;
                                                resy = 600;
                                            break;
                                            case 2:
                                                resx = 1024;
                                                resy = 768;
                                            break;
                                            case 3:
                                                resx = 1366;
                                                resy = 768;
                                            break;
                                            default:
                                            break;
                                        }


                                    break;
                                    case 3:
                                        fullscreen = true;
                                        menu_option->changeMenuCaption("    < ON >",3);
                                    break;
                                    case 5:
                                        sounds = true;

                                        menu_option->changeMenuCaption("    < ON >",5);
                                    break;
                                }
                            }

                        break;
                        case PEGA_KEY_RETURN:
                        if(reboot_message)
                        {
                            quit=true;


                        }
                        if(main_menu)
                        {
                            switch(menu->getCursorPosition())
                            {
                                case 0:
                                    inGame();
                                break;
                                case 1:
                                    OptionMenu();
                                break;
                                case 2:
                                    quit=true;
                                break;
                                default:
                                    std::cout<<"HOW THE FUCK COULD U GET THIS?!"<<std::endl;
                                break;
                            }
                        }
                        else if(option_menu)
                        {
                            bool keepwiththis = true;
                            if(menu_option->getCursorPosition()==6)
                            {
                                std::fstream fa;

                                fa.open("options.txt",std::fstream::out);


                                fa<<resx<<std::endl;
                                fa<<resy<<std::endl;
                                if(fullscreen)
                                    fa<<"ON"<<std::endl;
                                else
                                    fa<<"OFF"<<std::endl;

                                if(sounds)
                                    fa<<"ON";
                                else
                                    fa<<"OFF";

                                fa.close();

                                mainMenu();
                                keepwiththis = false;
                                if(resx!=resx_bak || fullscreen!=fullscreen_bak || sounds!=sounds_bak)
                                    ShutGame();
                            }

                            if(keepwiththis)
                            {
                                if((menu_option->getCursorPosition()<6 && menu_option->getCursorPosition()%2==0) || menu_option->getCursorPosition()==0)
                                {
                                    menu_option->nextMenuPosition();
                                    changing_vals=true;
                                }
                                else
                                {
                                    menu_option->previousMenuPosition();
                                    changing_vals=false;
                                }
                            }

                        }
                        break;
                        default:
                        break;
                    }
                break;
                case PEGA_EVENT_KEYUP:
                    switch(event->key->code)
                    {
                        case PEGA_KEY_UP:
                        case PEGA_KEY_W:
                            up=false;
                        break;
                        case PEGA_KEY_DOWN:
                        case PEGA_KEY_S:
                            down=false;
                        break;
                        case PEGA_KEY_LEFT:
                        case PEGA_KEY_A:
                            left=false;
                        break;
                        case PEGA_KEY_RIGHT:
                        case PEGA_KEY_D:
                            right=false;
                        break;
                        case PEGA_KEY_Z:
                            z=false;
                            break;
                        default:
                        break;
                    }
                break;
                default:
                break;
            }
        }
    }


    delete dev;

    return 0;
}

void ShutGame()
{
    ingame = false;
    main_menu = false;
    option_menu = false;

    reboot_message = true;
}


void displayFunction()
{

    if(ingame)
    {


        //{deletamento navi
        if(!boat_deleted)
        {
            if(!boat->isAlive())
            {
                delete boat;
                boat = nullptr;
                boat_deleted = true;

            }
        }

        //}
        firstlevel();
        if(!boat_deleted)
        {
            if(z)
                boat->shotFromHere("right");
            if(up && boat->getPosy()>0)
                boat->moveBoat("up");
            if(down && boat->getPosy()<resy-50)
                boat->moveBoat("down");
            if(left && boat->getPosx()>0)
                boat->moveBoat("left");
            if(right && boat->getPosx()<resx-50)
                boat->moveBoat("right");


            if(boat->isAlive())
            {
                boat->draw(0);

                for(int i=0;i<boat->getAmmo();i++)
                {

                    if(boat->checkShottableShots(i))
                    {
                        boat->shot(i,"right");

                        for(int k=0;k<N_NAVI_NEMICHE_MODELLO_1;k++)
                        {
                            if(enemies[k])
                            {
                                if(collision(boat->shots[i]->hitbox,enemy[k]->hitbox))
                                {
                                    enemy[k]->die();
                                    boat->shots[i]->active=false;
                                }
                            }

                        }

                    }
                }
            }
        }


    }
    else if(main_menu)
        menu->draw();
    else if(reboot_message)
    {
        pegaTextureResource* text_reboot_msg[2];
        pegaAnimation* anim_reboot_msg[2];

        text_reboot_msg[0] = font_renderer->generateString("Restarting for Applying changes..");
        text_reboot_msg[1] = font_renderer->generateString("Press Enter to close the game.");

        anim_reboot_msg[0] = animation_manager->createAnimation();
        anim_reboot_msg[1] = animation_manager->createAnimation();

        anim_reboot_msg[0]->pushFrame(text_reboot_msg[0]);
        anim_reboot_msg[1]->pushFrame(text_reboot_msg[1]);

        anim_reboot_msg[0]->setHeight(20);
        anim_reboot_msg[1]->setHeight(20);


        anim_reboot_msg[0]->setPosition(10,resy/2);
        anim_reboot_msg[1]->setPosition(10,(resy/2)+40);


        anim_reboot_msg[0]->draw();
        anim_reboot_msg[1]->draw();
    }
    else
        menu_option->draw();
}

void mainMenu()
{

    ingame = false;
    main_menu = true;
    option_menu = false;


}
void inGame()
{


    ingame = true;
    main_menu = false;
    option_menu = false;

}
void OptionMenu()
{
    ingame = false;
    main_menu = false;
    option_menu = true;
}
