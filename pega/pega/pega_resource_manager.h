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

#ifndef _PEGA_RESOURCE_MANAGER_H_
#define _PEGA_RESOURCE_MANAGER_H_

#include "pega_logger.h"
#include "pega_resource_package.h"
#include "pega_texture_resource.h"
#include "pega_audio_resource.h"
#include "pega_font_resource.h"

#define freeVector(name) \
for(size_t i=0; i<name.size(); i++) \
{ \
    if(name[i]->reference_count>0) \
    { \
        snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "%s has still %lld references", name[i]->debug_name.c_str(), name[i]->reference_count); \
        this->logger->writeMessage(PEGA_LOGGER_WARNING, this->buffer); \
    } \
    delete name[i]; \
} \
name.clear();

class pegaResourceManager
{
protected:
    char buffer[_PEGA_MESSAGE_BUFFER_SIZE];
    pegaLogger* logger;
    unsigned long long int id_gen;
    int maxTextureSize;
    std::vector<pegaAudioResource*> audioResVector;
    std::vector<pegaTextureResource*> textureResVector;
    std::vector<pegaFontResource*> fontResVector;
    std::vector<pegaResourcePackage*> packageResVector;
    pegaResourceManager(pegaLogger* created_logger)
    {
        this->logger = created_logger;
        this->id_gen = 0ULL;
        this->maxTextureSize = 0;
    }
    ~pegaResourceManager()
    {
        freeVector(audioResVector);
        freeVector(textureResVector);
        freeVector(fontResVector);
        for(size_t i=0; i<packageResVector.size(); i++)
        {
            delete packageResVector[i];
        }
        packageResVector.clear();
    }
    pegaTextureResource* createString(SDL_Surface* image_data, std::string dName)
    {
        SDL_Surface* new_image_data = SDL_ConvertSurfaceFormat(image_data, SDL_PIXELFORMAT_RGBA8888, 0);
        if(!new_image_data)
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Unable to convert %s - %s", dName.c_str(), SDL_GetError());
            this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer);
            return nullptr;
        }
        GLuint texture = 0;
        glEnable(GL_TEXTURE_2D); pegaCheckGLError();
        glGenTextures(1, &texture); pegaCheckGLError();
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); pegaCheckGLError();
        glBindTexture(GL_TEXTURE_2D, texture); pegaCheckGLError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); pegaCheckGLError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); pegaCheckGLError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); pegaCheckGLError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); pegaCheckGLError();
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     _PEGA_TEXTURE_INTERNAL_FORMAT,
                     new_image_data->w,
                     new_image_data->h,
                     0,
                     _PEGA_TEXTURE_FORMAT,
                     _PEGA_TEXTURE_TYPE,
                     new_image_data->pixels); pegaCheckGLError();
        glFinish(); pegaCheckGLError();
        glBindTexture(GL_TEXTURE_2D, 0); pegaCheckGLError();
        pegaTextureResource* temp = new pegaTextureResource(texture, new_image_data->w, new_image_data->h, this->logger, dName, this->id_gen++);
        SDL_FreeSurface(new_image_data);
        this->textureResVector.push_back(temp);
        return temp;
    }
public:
    pegaFontResource* createFontResource(const char* file_name, int pixel_height = 42, pegaResourcePackage* package = nullptr)
    {
        SDL_RWops* file_stream = nullptr;
        std::vector<unsigned char> raw_data;
        std::string dName;
        bool file_used;
        if(package)
        {
            dName.append(package->debug_name);
            dName.append(" -> ");
        }
        dName.append(file_name);
        if(package)
        {
            if(!package->getRAWData(file_name, raw_data))
            {
                snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Unable to open %s", dName.c_str());
                this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer);
                return nullptr;
            }
            file_used = false;
        }
        else
        {
            file_used = true;
            file_stream = SDL_RWFromFile(file_name, "rb");
        }
        if(!file_stream && file_used)
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Unable to open %s - %s", dName.c_str(), SDL_GetError());
            this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer);
            return nullptr;
        }
        pegaFontResource* temp = new pegaFontResource(this->logger, dName, this->id_gen++, file_used, file_stream, raw_data, pixel_height);
        this->fontResVector.push_back(temp);
        raw_data.clear();
        return temp;
    }
    pegaTextureResource* createTextureResource(const char* file_name, pegaResourcePackage* package = nullptr)
    {
        if(this->maxTextureSize == 0)
        {
            glGetIntegerv(GL_MAX_TEXTURE_SIZE, &(this->maxTextureSize));
        }
        SDL_RWops* file_stream = nullptr;
        std::vector<unsigned char> raw_data;
        std::string dName;
        if(package)
        {
            dName.append(package->debug_name);
            dName.append(" -> ");
        }
        dName.append(file_name);
        if(package)
        {
            if(!package->getRAWData(file_name, raw_data))
            {
                snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Unable to open %s", dName.c_str());
                this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer);
                return nullptr;
            }
            file_stream = SDL_RWFromMem(raw_data.data(), (int)raw_data.size());
        }
        else
        {
            file_stream = SDL_RWFromFile(file_name, "rb");
        }
        if(!file_stream)
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Unable to open %s - %s", dName.c_str(), SDL_GetError());
            this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer);
            return nullptr;
        }
        SDL_Surface* image_data = IMG_Load_RW(file_stream, 0);
        if(!image_data)
        {
            SDL_RWclose(file_stream);
            raw_data.clear();
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Unable to open %s - %s", dName.c_str(), IMG_GetError());
            this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer);
            return nullptr;
        }
        if(image_data->w > this->maxTextureSize || image_data->h > this->maxTextureSize)
        {
            SDL_RWclose(file_stream);
            raw_data.clear();
            SDL_FreeSurface(image_data);
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "%s is too large", dName.c_str());
            this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer);
            return nullptr;
        }
        SDL_RWclose(file_stream);
        raw_data.clear();
        SDL_Surface* new_image_data = SDL_ConvertSurfaceFormat(image_data, SDL_PIXELFORMAT_RGBA8888, 0);
        SDL_FreeSurface(image_data);
        if(!new_image_data)
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Unable to convert %s - %s", dName.c_str(), SDL_GetError());
            this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer);
            return nullptr;
        }
        GLuint texture = 0;
        glEnable(GL_TEXTURE_2D); pegaCheckGLError();
        glGenTextures(1, &texture); pegaCheckGLError();
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); pegaCheckGLError();
        glBindTexture(GL_TEXTURE_2D, texture); pegaCheckGLError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); pegaCheckGLError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); pegaCheckGLError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); pegaCheckGLError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); pegaCheckGLError();
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     _PEGA_TEXTURE_INTERNAL_FORMAT,
                     new_image_data->w,
                     new_image_data->h,
                     0,
                     _PEGA_TEXTURE_FORMAT,
                     _PEGA_TEXTURE_TYPE,
                     new_image_data->pixels); pegaCheckGLError();
        glFinish(); pegaCheckGLError();
        glBindTexture(GL_TEXTURE_2D, 0); pegaCheckGLError();
        pegaTextureResource* temp = new pegaTextureResource(texture, new_image_data->w, new_image_data->h, this->logger, dName, this->id_gen++);
        SDL_FreeSurface(new_image_data);
        this->textureResVector.push_back(temp);
        return temp;
    }
    pegaAudioResource* createAudioResource(const char* file_name, pegaResourcePackage* package = nullptr)
    {
        SDL_RWops* file_stream = nullptr;
        std::vector<unsigned char> raw_data;
        std::string dName;
        if(package)
        {
            dName.append(package->debug_name);
            dName.append(" -> ");
        }
        dName.append(file_name);
        if(package)
        {
            if(!package->getRAWData(file_name, raw_data))
            {
                snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Unable to open %s", dName.c_str());
                this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer);
                return nullptr;
            }
            file_stream = SDL_RWFromMem(raw_data.data(), (int)raw_data.size());
        }
        else
        {
            file_stream = SDL_RWFromFile(file_name, "rb");
        }
        if(!file_stream)
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Unable to open %s - %s", dName.c_str(), SDL_GetError());
            this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer);
            return nullptr;
        }
        Mix_Chunk* audio_data = Mix_LoadWAV_RW(file_stream, 0);
        if(!audio_data)
        {
            SDL_RWclose(file_stream);
            raw_data.clear();
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Unable to open %s - %s", dName.c_str(), Mix_GetError());
            this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer);
            return nullptr;
        }
        SDL_RWclose(file_stream);
        raw_data.clear();
        pegaAudioResource* temp = new pegaAudioResource(audio_data, this->logger, dName, this->id_gen++);
        this->audioResVector.push_back(temp);
        return temp;
    }
    pegaResourcePackage* openPackage(const char* archive_name)
    {
        int err;
        zip* archive = zip_open(archive_name, 0, &err);
        if(!archive)
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Unable to open %s", archive_name);
            this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer);
            return nullptr;
        }
        pegaResourcePackage* temp = new pegaResourcePackage(archive, archive_name, this->logger, this->id_gen++);
        this->packageResVector.push_back(temp);
        return temp;
    }
    void closePackage(pegaResourcePackage* package)
    {
        if(package==nullptr)
            return;
        for(std::vector<pegaResourcePackage*>::iterator it = this->packageResVector.begin(); it != this->packageResVector.end(); it++)
        {
            if(package->id == (*it)->id)
            {
                this->packageResVector.erase(it);
                break;
            }
        }
        delete package;
    }
    void deleteAudioResource(pegaAudioResource* resource, bool force = false)
    {
        if(resource==nullptr)
            return;
        if(!force)
        {
            if(resource->reference_count>0)
            {
                snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "%s cannot be free'd because has still %lld reference(s)", resource->debug_name.c_str(), resource->reference_count);
                this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer);
                return;
            }
        }
        else
        {
            if(resource->reference_count>0)
            {
                snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "%s had still %lld reference(s)", resource->debug_name.c_str(), resource->reference_count);
                this->logger->writeMessage(PEGA_LOGGER_WARNING, this->buffer);
                return;
            }
        }
        for(std::vector<pegaAudioResource*>::iterator it = this->audioResVector.begin(); it!=this->audioResVector.end(); it++)
        {
            if(resource->id == (*it)->id)
            {
                this->audioResVector.erase(it);
                break;
            }
        }
        delete resource;
        return;
    }
    void deleteTextureResource(pegaTextureResource* resource, bool force = false)
    {
        if(resource==nullptr)
            return;
        if(!force)
        {
            if(resource->reference_count>0)
            {
                snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "%s cannot be free'd because has still %lld reference(s)", resource->debug_name.c_str(), resource->reference_count);
                this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer);
                return;
            }
        }
        else
        {
            if(resource->reference_count>0)
            {
                snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "%s had still %lld reference(s)", resource->debug_name.c_str(), resource->reference_count);
                this->logger->writeMessage(PEGA_LOGGER_WARNING, this->buffer);
                return;
            }
        }
        for(std::vector<pegaTextureResource*>::iterator it = this->textureResVector.begin(); it!=this->textureResVector.end(); it++)
        {
            if(resource->id == (*it)->id)
            {
                this->textureResVector.erase(it);
                break;
            }
        }
        delete resource;
        return;
    }
    void deleteFontResource(pegaFontResource* resource, bool force = false)
    {
        if(resource==nullptr)
            return;
        if(!force)
        {
            if(resource->reference_count>0)
            {
                snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "%s cannot be free'd because has still %lld reference(s)", resource->debug_name.c_str(), resource->reference_count);
                this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer);
                return;
            }
        }
        else
        {
            if(resource->reference_count>0)
            {
                snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "%s had still %lld reference(s)", resource->debug_name.c_str(), resource->reference_count);
                this->logger->writeMessage(PEGA_LOGGER_WARNING, this->buffer);
                return;
            }
        }
        for(std::vector<pegaFontResource*>::iterator it = this->fontResVector.begin(); it!=this->fontResVector.end(); it++)
        {
            if(resource->id == (*it)->id)
            {
                this->fontResVector.erase(it);
                break;
            }
        }
        delete resource;
        return;
    }
    friend class pegaDevice;
    friend class pegaFontRenderer;
};

#undef freeVector

#endif

#endif
