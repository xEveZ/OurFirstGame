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

#ifndef _PEGA_EVENT_MOUSE_H_
#define _PEGA_EVENT_MOUSE_H_

#define PEGA_BUTTON_LEFT SDL_BUTTON_LEFT
#define PEGA_BUTTON_RIGHT SDL_BUTTON_RIGHT
#define PEGA_BUTTON_MIDDLE SDL_BUTTON_MIDDLE
#define PEGA_BUTTON_OTHER1 SDL_BUTTON_X1
#define PEGA_BUTTON_OTHER2 SDL_BUTTON_X2

class pegaEventMouseClick
{
protected:
    pegaEventMouseClick(){}
    ~pegaEventMouseClick(){}
public:
    int button;
    friend class pegaEvent;
};

class pegaEventMouseMotion
{
protected:
    pegaEventMouseMotion(){}
    ~pegaEventMouseMotion(){}
public:
    int x;
    int y;
    int x_relative;
    int y_relative;
    friend class pegaEvent;
};

class pegaEventMouseWheel
{
protected:
    pegaEventMouseWheel(){}
    ~pegaEventMouseWheel(){}
public:
    int x;
    int y;
    friend class pegaEvent;
};

#endif

#endif
