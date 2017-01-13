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

#ifndef _PEGA_FONT_RESOURCE_H_
#define _PEGA_FONT_RESOURCE_H_

#include "pega_logger.h"

class pegaFontResource
{
protected:
    char buffer[_PEGA_MESSAGE_BUFFER_SIZE];
    pegaLogger* logger;
    std::string debug_name;
    unsigned long long int id;
    long long int reference_count;
    TTF_Font* font;
    SDL_RWops* file_stream;
    std::vector<unsigned char> data;
    pegaFontResource(pegaLogger* created_logger, std::string dName, unsigned long long int my_id, bool file_res, SDL_RWops* fs, std::vector<unsigned char>& raw_data, int px)
    {
        this->logger = created_logger;
        this->debug_name = dName;
        this->id = my_id;
        this->reference_count = 0LL;
        if(file_res)
        {
            this->file_stream = fs;
            this->font = TTF_OpenFontRW(this->file_stream, 0, px);
        }
        else
        {
            this->data.insert(this->data.end(), raw_data.begin(), raw_data.end());
            this->file_stream = SDL_RWFromMem(this->data.data(), (int)this->data.size());
            if(!this->file_stream)
            {
                snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Unable to open %s - %s", dName.c_str(), SDL_GetError());
                this->logger->writeMessage(PEGA_LOGGER_CRITICAL, this->buffer);
                exit(EXIT_FAILURE);
            }
            this->font = TTF_OpenFontRW(this->file_stream, 0, px);
        }
        if(!this->font)
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Unable to open %s - %s", dName.c_str(), TTF_GetError());
            this->logger->writeMessage(PEGA_LOGGER_CRITICAL, this->buffer);
            exit(EXIT_FAILURE);
        }
    }
    ~pegaFontResource()
    {
        TTF_CloseFont(this->font);
        SDL_RWclose(this->file_stream);
        data.clear();
    }
public:
    friend class pegaResourceManager;
    friend class pegaResourcePackage;
    friend class pegaFontRenderer;
};

#endif

#endif
