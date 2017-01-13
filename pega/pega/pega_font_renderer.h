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

#ifndef _PEGA_FONT_RENDERER_H_
#define _PEGA_FONT_RENDERER_H_

#include "pega_logger.h"

#define PEGA_FONT_QUALITY_LOW 0
#define PEGA_FONT_QUALITY_MEDIUM 1
#define PEGA_FONT_QUALITY_HIGH 2

class pegaFontRenderer
{
protected:
    char buffer[_PEGA_MESSAGE_BUFFER_SIZE];
    pegaLogger* logger;
    pegaResourceManager* manager;
    pegaFontResource* font;
    int outline;
    SDL_Color color;
    SDL_Color black;
    int quality;
    pegaFontRenderer(pegaLogger* created_logger, pegaResourceManager* r_manager)
    {
        this->logger = created_logger;
        this->manager = r_manager;
        this->font = nullptr;
        this->outline = 0;
        this->color.a = 255;
        this->color.r = 255;
        this->color.g = 255;
        this->color.b = 255;
        this->black.a = 0;
        this->black.r = 0;
        this->black.g = 0;
        this->black.b = 0;
        this->quality = PEGA_FONT_QUALITY_HIGH;
    }
    ~pegaFontRenderer()
    {
        if(this->font)
        {
            this->font->reference_count--;
        }
    }
public:
    pegaTextureResource* generateString(const char* s)
    {
        if(!this->font)
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "No font is set");
            this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer);
            return nullptr;
        }
        SDL_Surface* surf = nullptr;
        if(this->quality == PEGA_FONT_QUALITY_LOW)
        {
            #ifdef PEGA_OS_WINDOWS
            surf = TTF_RenderText_Solid(this->font->font, s, this->color);
            #else
            surf = TTF_RenderUTF8_Solid(this->font->font, s, this->color);
            #endif
        }
        else if(this->quality == PEGA_FONT_QUALITY_MEDIUM)
        {
            #ifdef PEGA_OS_WINDOWS
            surf = TTF_RenderText_Shaded(this->font->font, s, this->color, this->black);
            #else
            surf = TTF_RenderUTF8_Shaded(this->font->font, s, this->color, this->black);
            #endif
        }
        else
        {
            #ifdef PEGA_OS_WINDOWS
            surf = TTF_RenderText_Blended(this->font->font, s, this->color);
            #else
            surf = TTF_RenderUTF8_Blended(this->font->font, s, this->color);
            #endif
        }
        if(!surf)
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Unable to render \"%s\"", s);
            this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer);
            return nullptr;
        }
        pegaTextureResource* temp = this->manager->createString(surf, this->font->debug_name);
        SDL_FreeSurface(surf);
        return temp;
    }
    void setQuality(int q)
    {
        this->quality = q;
    } ///< PEGA_FONT_QUALITY_LOW, PEGA_FONT_QUALITY_MEDIUM or PEGA_FONT_QUALITY_HIGH
    void setOutline(int pixel)
    {
        if(pixel<0)
            pixel = 0;
        this->outline = pixel;
        if(this->font)
            TTF_SetFontOutline(this->font->font, this->outline);
    }
    void setFontResource(pegaFontResource* resource)
    {
        if(this->font)
        {
            this->font->reference_count--;
        }
        this->font = resource;
        if(this->font)
        {
            this->font->reference_count++;
            TTF_SetFontKerning(this->font->font, 1);
            TTF_SetFontOutline(this->font->font, this->outline);
        }
    }
    friend class pegaDevice;
};

#endif

#endif
