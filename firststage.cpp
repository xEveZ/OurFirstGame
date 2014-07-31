#include "firststage.h"

nave* enemy[N_NAVI_NEMICHE_MODELLO_1];
bool enemies[N_NAVI_NEMICHE_MODELLO_1];

pegaTimer timer_scritta;
pegaAnimation* Stage_one;
pegaTextureResource* text_Stage_one[31];

bool start_level=false;
bool first_time_lv1=true;
bool letsgorangers=false;

void setlevel_one()
{
    for(uint8_t i=0;i<N_NAVI_NEMICHE_MODELLO_1;i++)
    {
        enemy[i] = new nave(49,29,100,30,1.5,300);


        enemy[i]->setAnimation(animation_enemy[i]);
        enemy[i]->setScale(30);

        if(i==1)
            enemy[i]->setPosx(resx+(200));
        else
            enemy[i]->setPosx(resx+100);

        enemy[i]->setPosy((i*200)+90);

        enemies[i]=true;

    }
    Stage_one = animation_manager->createAnimation();
    for(int i=0;i<31;i++)
    {
        std::string app;
        app = "./img/Firststage[" + convertInt(i) + "].png";
        text_Stage_one[i] = res->createTextureResource(app.c_str());

        if(i==18)
        {
            for(int k=0;k<60;k++)
            {
                Stage_one->pushFrame(text_Stage_one[i]);
            }
        }
        else
        {
            Stage_one->pushFrame(text_Stage_one[i]);
        }


    }

    Stage_one->setBlending(true);
    Stage_one->setFPS(30);
    Stage_one->setPosition(0,0);
    Stage_one->setLoop(false);
    timer_scritta.reset();
}


void firstlevel()
{
    if(first_time_lv1)
    {
        Stage_one->reset();
        first_time_lv1=false;
    }



    if(timer_scritta.getMilliSeconds()<4000)
    {
        Stage_one->draw();
    }
    else
    {


        if(!start_level)//blocchiamo sfondo e navi e resettiamo i timer per muoverli quando deve diventare true
        {
            timer.reset();

            for(int i=0;i<N_NAVI_NEMICHE_MODELLO_1;i++)
            {
                if(enemies[i])
                {
                    enemy[i]->setupBot();
                }

            }
            start_level=true;
        }



        background_anim->setPosition(backround_x_variable,0);
        backround_x_variable -= double(timer.getMilliSeconds())*0.003;
        timer.reset();
        background_anim->draw();
        if(timer_scritta.getMilliSeconds()>8000)
        {
            if(!letsgorangers)
            {
                for(int i=0;i<N_NAVI_NEMICHE_MODELLO_1;i++)
                {
                    if(enemies[i])
                        enemy[i]->setupBot();
                }
                letsgorangers=true;
            }





            for(int i=0;i<N_NAVI_NEMICHE_MODELLO_1;i++)
            {
                if(enemies[i])
                {
                    if(enemy[i]->isAlive())
                    {
                        enemy[i]->draw(0);
                        enemy[i]->moveBoat("left",true,-200);

                        if(enemy[i]->getPosx()<-200)
                        {
                            if(i==1)
                                enemy[i]->setPosx(resx+(100));
                            else
                                enemy[i]->setPosx(resx-50);

                            enemy[i]->setPosy((i*200)+90);
                        }
                    }
                    else
                    {
                        enemies[i] = false;
                        delete enemy[i];
                        enemy[i] = nullptr;
                    }
                }




            }


        }

    }

}




