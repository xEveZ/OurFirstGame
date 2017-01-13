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

#ifndef _PEGA_DEVICE_H_
#define _PEGA_DEVICE_H_

#include "pega_logger.h"
#include "pega_window.h"
#include "pega_resource_manager.h"
#include "pega_mixer.h"
#include "pega_animation_manager.h"
#include "pega_font_renderer.h"

#define PEGA_ANTIALIASING_NONE 0
#define PEGA_ANTIALIASING_LOW 1
#define PEGA_ANTIALIASING_MEDIUM 1
#define PEGA_ANTIALIASING_HIGH 4

class pegaDevice
{
protected:
    pegaLogger* logger;
    pegaWindow* default_window;
    pegaResourceManager* resource_manager;
    pegaMixer* audio_mixer;
    pegaAnimationManager* animation_manager;
    pegaFontRenderer* font_renderer;
    bool internal_logger;
    bool window_already_created;
    char buffer[_PEGA_MESSAGE_BUFFER_SIZE];
public:
    pegaWindow* createWindow(const char* title = "(default)", int antialiasing = PEGA_ANTIALIASING_NONE, bool fullscreen = false, int width = -1, int height = -1)
    {
        if(this->window_already_created)
        {
            this->logger->writeMessage(PEGA_LOGGER_WARNING, "The window has already been created");
            return nullptr;
        }
        this->window_already_created = true;
        if(fullscreen && (width==-1 || height==-1))
        {
            SDL_DisplayMode vm;
            if(SDL_GetCurrentDisplayMode(0, &vm))
            {
                this->logger->writeMessage(PEGA_LOGGER_CRITICAL, SDL_GetError());
                exit(EXIT_FAILURE);
            }
            width = vm.w;
            height = vm.h;
        }
        else if(width==-1 || height==-1)
        {
            width = 800;
            height = 600;
        }
        return this->default_window = new pegaWindow(title, fullscreen, width, height, antialiasing, this->logger);
    } ///< PEGA_ANTIALIASING_NONE, PEGA_ANTIALIASING_LOW, PEGA_ANTIALIASING_MEDIUM or PEGA_ANTIALIASING_HIGH
    pegaDevice(int* pArgc = nullptr, char*** pArgv = nullptr, pegaLogger* pLogger = nullptr)
    {
        (void)pArgc; (void)pArgv;
        int flags = 0;
        int return_code = 0;
        if(pLogger)
        {
            this->logger = pLogger;
            this->internal_logger = false;
        }
        else
        {
            logger = new pegaLogger();
            this->internal_logger = true;
        }
        this->window_already_created = false;
        if(SDL_Init(SDL_INIT_EVERYTHING))
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Unable to load window library support - %s", SDL_GetError());
            this->logger->writeMessage(PEGA_LOGGER_CRITICAL, this->buffer);
            exit(EXIT_FAILURE);
        }
        flags = MIX_INIT_FLAC | MIX_INIT_OGG;
        return_code = Mix_Init(flags);
        if((flags&return_code) != flags)
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Unable to load audio support - %s", Mix_GetError());
            this->logger->writeMessage(PEGA_LOGGER_CRITICAL, this->buffer);
            exit(EXIT_FAILURE);
        }
        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024))
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Unable to load audio support - %s", Mix_GetError());
            this->logger->writeMessage(PEGA_LOGGER_CRITICAL, this->buffer);
            exit(EXIT_FAILURE);
        }
        flags = IMG_INIT_JPG | IMG_INIT_PNG;
        return_code = IMG_Init(flags);
        if((flags&return_code) != flags)
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Unable to load image support - %s", IMG_GetError());
            this->logger->writeMessage(PEGA_LOGGER_CRITICAL, this->buffer);
            exit(EXIT_FAILURE);
        }
        if(TTF_Init())
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Unable to load font support - %s", TTF_GetError());
            this->logger->writeMessage(PEGA_LOGGER_CRITICAL, this->buffer);
            exit(EXIT_FAILURE);
        }
        this->resource_manager = new pegaResourceManager(this->logger);
        this->audio_mixer = new pegaMixer(this->logger);
        this->animation_manager = new pegaAnimationManager(this->logger);
        this->font_renderer = new pegaFontRenderer(this->logger, this->resource_manager);
        snprintf(this->buffer,
                 _PEGA_MESSAGE_BUFFER_SIZE,
                 "Powered by Pegasus Engine v%d.%d.%d %s",
                 PEGA_VERSION_MAJOR,
                 PEGA_VERSION_MINOR,
                 PEGA_VERSION_PATCH,
                 PEGA_VERSION_OTHER);
        this->logger->writeMessage(PEGA_LOGGER_INFORMATION, this->buffer);
    }
    ~pegaDevice()
    {
        delete this->audio_mixer;
        delete this->animation_manager;
        delete this->font_renderer;
        delete this->resource_manager;
        if(this->window_already_created)
        {
            delete this->default_window;
        }
        TTF_Quit();
        IMG_Quit();
        Mix_CloseAudio();
        Mix_Quit();
        SDL_Quit();
        if(internal_logger)
            delete this->logger;
    }
    pegaResourceManager* getResourceManager()
    {
        if(!this->window_already_created)
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "The window must be created before getting the resource manager");
            this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer);
            return nullptr;
        }
        return this->resource_manager;
    }
    pegaAnimationManager* getAnimationManager()
    {
        if(!this->window_already_created)
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "The window must be created before getting the animation manager");
            this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer);
            return nullptr;
        }
        return this->animation_manager;
    }
    pegaFontRenderer* getFontRenderer()
    {
        if(!this->window_already_created)
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "The window must be created before getting the font renderer");
            this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer);
            return nullptr;
        }
        return this->font_renderer;
    }
    pegaMixer* getMixer()
    {
        return this->audio_mixer;
    }
    int getScreenWidth()
    {
        SDL_DisplayMode vm;
        if(SDL_GetCurrentDisplayMode(0, &vm))
        {
            this->logger->writeMessage(PEGA_LOGGER_CRITICAL, SDL_GetError());
            exit(EXIT_FAILURE);
        }
        return vm.w;
    }
    int getScreenHeight()
    {
        SDL_DisplayMode vm;
        if(SDL_GetCurrentDisplayMode(0, &vm))
        {
            this->logger->writeMessage(PEGA_LOGGER_CRITICAL, SDL_GetError());
            exit(EXIT_FAILURE);
        }
        return vm.h;
    }
};

#endif

#endif
