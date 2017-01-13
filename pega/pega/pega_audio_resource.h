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

#ifndef _PEGA_AUDIO_RESOURCE_H_
#define _PEGA_AUDIO_RESOURCE_H_

class pegaAudioResource
{
protected:
    char buffer[_PEGA_MESSAGE_BUFFER_SIZE];
    pegaLogger* logger;
    std::string debug_name;
    unsigned long long int id;
    long long int reference_count;
    Mix_Chunk* audio_data;
    pegaAudioResource(Mix_Chunk* chunk, pegaLogger* created_logger, std::string dName, unsigned long long int my_id)
    {
        this->audio_data = chunk;
        this->logger = created_logger;
        this->debug_name = dName;
        this->id = my_id;
        this->reference_count = 0LL;
        Mix_VolumeChunk(this->audio_data, MIX_MAX_VOLUME);
    }
    ~pegaAudioResource()
    {
        Mix_FreeChunk(this->audio_data);
    }
public:
    friend class pegaResourceManager;
    friend class pegaResourcePackage;
    friend class pegaMixerChannel;
};

#endif

#endif
