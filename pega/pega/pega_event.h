/*
Pegasus Engine
Copyright (c) 2014, Dario Ostuni
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL DARIO OSTUNI BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#if defined _PEGA_H_

#ifndef _PEGA_EVENT_H_
#define _PEGA_EVENT_H_

#include "pega_logger.h"
#include "pega_event_keyboard.h"
#include "pega_event_mouse.h"
#include "pega_event_joystick.h"

#define PEGA_EVENT_IGNORE -1
#define PEGA_EVENT_QUIT 0
#define PEGA_EVENT_KEYDOWN 1
#define PEGA_EVENT_KEYUP 2
#define PEGA_EVENT_MOUSEMOTION 3
#define PEGA_EVENT_MOUSEWHEEL 4
#define PEGA_EVENT_MOUSEBUTTONDOWN 5
#define PEGA_EVENT_MOUSEBUTTONUP 6
#define PEGA_EVENT_JOYAXIS 7
#define PEGA_EVENT_JOYBUTTONDOWN 8
#define PEGA_EVENT_JOYBUTTONUP 9
#define PEGA_EVENT_JOYDPAD 10

class pegaEvent
{
protected:
    SDL_Event* sdlEvent;
    char buffer[_PEGA_MESSAGE_BUFFER_SIZE];
    pegaLogger* logger;
    pegaEventKeyboard int_key;
    pegaEventMouseClick int_click;
    pegaEventMouseMotion int_motion;
    pegaEventMouseWheel int_wheel;
    pegaEventJoyAxis int_axis;
    pegaEventJoyButton int_button;
    pegaEventJoyDPad int_dpad;
    pegaEvent(pegaLogger* created_logger)
    {
        this->logger = created_logger;
        this->key = &(this->int_key);
        this->click = &(this->int_click);
        this->motion = &(this->int_motion);
        this->wheel = &(this->int_wheel);
        this->joyaxis = &(this->int_axis);
        this->joybutton = &(this->int_button);
        this->joydpad = &(this->int_dpad);
    }
    void createEvent(SDL_Event* e)
    {
        this->sdlEvent = e;
        // SWITCH DEL TIPO DI EVENTO
        switch(e->type)
        {
            case SDL_QUIT:
                this->type = PEGA_EVENT_QUIT;
            break;

            case SDL_KEYDOWN:
                this->type = PEGA_EVENT_KEYDOWN;
                int_key.setKey(e);
            break;

            case SDL_KEYUP:
                this->type = PEGA_EVENT_KEYUP;
                int_key.setKey(e);
            break;

            case SDL_MOUSEMOTION:
                this->type = PEGA_EVENT_MOUSEMOTION;
                this->int_motion.x = e->motion.x;
                this->int_motion.y = e->motion.y;
                this->int_motion.x_relative = e->motion.xrel;
                this->int_motion.y_relative = e->motion.yrel;
            break;

            case SDL_MOUSEWHEEL:
                this->type = PEGA_EVENT_MOUSEWHEEL;
                this->int_wheel.x = e->wheel.x;
                this->int_wheel.y = e->wheel.y;
            break;

            case SDL_MOUSEBUTTONDOWN:
                this->type = PEGA_EVENT_MOUSEBUTTONDOWN;
                this->int_click.button = e->button.button;
            break;

            case SDL_MOUSEBUTTONUP:
                this->type = PEGA_EVENT_MOUSEBUTTONUP;
                this->int_click.button = e->button.button;
            break;

            case SDL_JOYAXISMOTION:
                this->type = PEGA_EVENT_JOYAXIS;
                this->int_axis.id = e->jaxis.which;
                this->int_axis.axis = e->jaxis.axis;
                if(e->jaxis.value<0)
                    this->int_axis.value = double(e->jaxis.value)/32768.0;
                else
                    this->int_axis.value = double(e->jaxis.value)/32767.0;
            break;

            case SDL_JOYBUTTONDOWN:
                this->type = PEGA_EVENT_JOYBUTTONDOWN;
                this->int_button.id = e->jbutton.which;
                this->int_button.button = e->jbutton.button;
            break;

            case SDL_JOYBUTTONUP:
                this->type = PEGA_EVENT_JOYBUTTONUP;
                this->int_button.id = e->jbutton.which;
                this->int_button.button = e->jbutton.button;
            break;

            case SDL_JOYHATMOTION:
                this->type = PEGA_EVENT_JOYDPAD;
                this->int_dpad.id = e->jhat.which;
                switch(e->jhat.value)
                {
                    case SDL_HAT_LEFTUP:
                        this->int_dpad.x = -1;
                        this->int_dpad.y = -1;
                    break;

                    case SDL_HAT_LEFTDOWN:
                        this->int_dpad.x = -1;
                        this->int_dpad.y = 1;
                    break;

                    case SDL_HAT_UP:
                        this->int_dpad.x = 0;
                        this->int_dpad.y = -1;
                    break;

                    case SDL_HAT_DOWN:
                        this->int_dpad.x = 0;
                        this->int_dpad.y = 1;
                    break;

                    case SDL_HAT_RIGHTUP:
                        this->int_dpad.x = 1;
                        this->int_dpad.y = -1;
                    break;

                    case SDL_HAT_RIGHTDOWN:
                        this->int_dpad.x = 1;
                        this->int_dpad.y = 1;
                    break;

                    case SDL_HAT_LEFT:
                        this->int_dpad.x = -1;
                        this->int_dpad.y = 0;
                    break;

                    case SDL_HAT_RIGHT:
                        this->int_dpad.x = 1;
                        this->int_dpad.y = 0;
                    break;

                    case SDL_HAT_CENTERED:
                        this->int_dpad.x = 0;
                        this->int_dpad.y = 0;
                    break;
                }
            break;

            default:
                this->type = PEGA_EVENT_IGNORE;
            break;
        }

    }
public:
    int type;
    pegaEventKeyboard* key; ///< PEGA_KEY_
    pegaEventMouseClick* click; ///< PEGA_BUTTON_
    pegaEventMouseMotion* motion;
    pegaEventMouseWheel* wheel;
    pegaEventJoyAxis* joyaxis;
    pegaEventJoyButton* joybutton;
    pegaEventJoyDPad* joydpad;
    friend class pegaWindow;
};

#endif

#endif
