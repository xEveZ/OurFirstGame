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

#ifndef _PEGA_WINDOW_H_
#define _PEGA_WINDOW_H_

#include "pega_logger.h"
#include "pega_event.h"

#define PEGA_MESSAGEBOX_INFORMATION SDL_MESSAGEBOX_INFORMATION
#define PEGA_MESSAGEBOX_WARNING SDL_MESSAGEBOX_WARNING
#define PEGA_MESSAGEBOX_ERROR SDL_MESSAGEBOX_ERROR

class pegaWindow
{
protected:
    SDL_Event e;
    pegaEvent pe;
    SDL_Window* window;
    SDL_GLContext glcon;
    int w;
    int h;
    pegaLogger* logger;
    std::vector<std::pair<void*, bool> > joyv;
    char buffer[_PEGA_MESSAGE_BUFFER_SIZE];
    void checkGLExtension(const char* ext)
    {
        if(!glewIsSupported(ext))
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "%s is not supported", ext);
            this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer);
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "The video driver does not support the OpenGL extensions required by the Pegasus Engine; please update your video driver");
            this->logger->writeMessage(PEGA_LOGGER_CRITICAL, this->buffer);
            exit(EXIT_FAILURE);
        }
        return;
    }
    pegaWindow(const char* title, bool fullscreen, int width, int height, int antialiasing, pegaLogger* created_logger)
    : pe(created_logger)
    {
        this->w = width;
        this->h = height;
        this->logger = created_logger;
        if(antialiasing)
        {
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, antialiasing);
        }
        this->window = SDL_CreateWindow(title,
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        width,
                                        height,
                                        SDL_WINDOW_OPENGL | ((fullscreen) ? (SDL_WINDOW_FULLSCREEN) : (0)));
        if(!this->window)
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Could not create the window - %s", SDL_GetError());
            this->logger->writeMessage(PEGA_LOGGER_CRITICAL, this->buffer);
            exit(EXIT_FAILURE);
        }
        this->glcon = SDL_GL_CreateContext(this->window);
        if(!glcon)
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Could not create the OpenGL context - %s", SDL_GetError());
            this->logger->writeMessage(PEGA_LOGGER_CRITICAL, this->buffer);
            exit(EXIT_FAILURE);
        }
        if(glewInit()!=GLEW_OK)
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Could not create the OpenGL extensions wrangler");
            this->logger->writeMessage(PEGA_LOGGER_CRITICAL, this->buffer);
            exit(EXIT_FAILURE);
        }
        snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "OpenGL version: %s", ((glGetString(GL_VERSION)) ? ((const char*)glGetString(GL_VERSION)) : ("(null)")));  pegaCheckGLError();
        this->logger->writeMessage(PEGA_LOGGER_INFORMATION, this->buffer);
        snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "OpenGL vendor: %s", ((glGetString(GL_VENDOR)) ? ((const char*)glGetString(GL_VENDOR)) : ("(null)")));  pegaCheckGLError();
        this->logger->writeMessage(PEGA_LOGGER_INFORMATION, this->buffer);
        this->checkGLExtension("GL_VERSION_2_1");
        this->checkGLExtension("GL_ARB_texture_non_power_of_two");
        glEnable(GL_TEXTURE_2D); pegaCheckGLError();
        glAlphaFunc(GL_GREATER, 0.0f); pegaCheckGLError();
        glEnable(GL_ALPHA_TEST); pegaCheckGLError();
        int numJoy = SDL_NumJoysticks();
        for(int i = 0; i<numJoy; i++)
        {
            SDL_GameController* tempcon = SDL_GameControllerOpen(i);
            if(!tempcon)
            {
                snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Could not open GameController #%d - %s", i, SDL_GetError());
                this->logger->writeMessage(PEGA_LOGGER_DEBUG, this->buffer);
            }
            else
            {
                this->joyv.push_back(std::make_pair((void*)tempcon, true));
                continue;
            }
            SDL_Joystick* tempjoy = SDL_JoystickOpen(i);
            if(!tempjoy)
            {
                snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Could not open JoyStick #%d - %s", i, SDL_GetError());
                this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer);
            }
            else
            {
                this->joyv.push_back(std::make_pair((void*)tempjoy, false));
            }
        }
    }
    ~pegaWindow()
    {
        for(size_t i=0; i<this->joyv.size(); i++)
        {
            if(this->joyv[i].second)
            {
                SDL_GameControllerClose((SDL_GameController*)this->joyv[i].first);
            }
            else
            {
                SDL_JoystickClose((SDL_Joystick*)this->joyv[i].first);
            }
        }
        this->joyv.clear();
        SDL_GL_DeleteContext(this->glcon);
        SDL_DestroyWindow(this->window);
    }
    float clamp01(float number)
    {
        if(number<0.0f)
            number = 0.0f;
        else if(number>1.0f)
            number = 1.0f;
        return number;
    }
public:
    void showCursor(bool toggle = true)
    {
        if(SDL_ShowCursor(((toggle) ? (1) : (0)))<0)
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Unable to change cursor state - %s", SDL_GetError());
            this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer);
        }
    }
    void setBackgroundColor(float red, float green, float blue, float alpha = 1.0f)
    {
        red = this->clamp01(red);
        green = this->clamp01(green);
        blue = this->clamp01(blue);
        alpha = this->clamp01(alpha);
        glClearColor(red, green, blue, alpha);  pegaCheckGLError();
    }
    void beginScene()
    {
        glClear(GL_COLOR_BUFFER_BIT); pegaCheckGLError();
    }
    void endScene()
    {
        glFlush(); pegaCheckGLError();
        SDL_GL_SwapWindow(this->window);
    }
    pegaEvent* getEvent()
    {
        int return_code = SDL_PollEvent(&(this->e));
        if(return_code)
        {
            this->pe.createEvent(&(this->e));
            return &(this->pe);
        }
        else
        {
            return nullptr;
        }
    }
    int getWidth()
    {
        return this->w;
    }
    int getHeight()
    {
        return this->h;
    }
    void warpMouse(int x, int y)
    {
        SDL_WarpMouseInWindow(this->window, x, y);
    }
    void setRelativeMouseMotion(bool toggle = true)
    {
        if(SDL_SetRelativeMouseMode(((toggle) ? (SDL_TRUE) : (SDL_FALSE))))
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Unable to set relative mouse motion - %s", SDL_GetError());
            this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer);
        }
    }
    void showMessageBox(unsigned int type, const char* title, const char* message)
    {
        if(SDL_ShowSimpleMessageBox(type, title, message, this->window)<0)
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Unable to create the MessageBox - %s", SDL_GetError());
            this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer);
        }
    } ///< PEGA_MESSAGEBOX_INFORMATION, PEGA_MESSAGEBOX_WARNING or PEGA_MESSAGEBOX_ERROR
    bool isActive()
    {
        return bool(SDL_WINDOW_SHOWN & SDL_GetWindowFlags(this->window));
    }
    void yield()
    {
        SDL_Delay(0U);
    }
    void enableVSync(bool toggle = true)
    {
        if(toggle)
        {
            if(SDL_GL_SetSwapInterval(-1)<0)
                SDL_GL_SetSwapInterval(1);
        }
        else
        {
            SDL_GL_SetSwapInterval(0);
        }
    }
    friend class pegaDevice;
};

#endif

#endif
