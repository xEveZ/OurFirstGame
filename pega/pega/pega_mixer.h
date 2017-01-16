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

#ifndef _PEGA_MIXER_H_
#define _PEGA_MIXER_H_

#include "pega_mixer_channel.h"
#include "pega_logger.h"

class pegaMixer
{
protected:
    char buffer[_PEGA_MESSAGE_BUFFER_SIZE];
    pegaLogger* logger;
    std::vector<pegaMixerChannel*> taken;
    double master_volume;
    pegaMixer(pegaLogger* created_logger)
    {
        this->logger = created_logger;
        this->taken.resize(Mix_AllocateChannels(64), nullptr);
        this->master_volume = 1.0;
    }
    ~pegaMixer()
    {
        for(int i=0; i<(int)this->taken.size(); i++)
        {
            if(taken[i])
            {
                delete taken[i];
                taken[i] = nullptr;
            }
        }
    }
public:
    void setMasterVolume(double volume)
    {
        if(volume>1.0)
            volume = 1.0;
        if(volume<0.0)
            volume = 0.0;
        this->master_volume = volume;
        for(size_t i=0; i<this->taken.size(); i++)
        {
            if(taken[i])
            {
                taken[i]->master_volume = this->master_volume;
                taken[i]->calcVolume();
            }
        }
    }
    pegaMixerChannel* createChannel(pegaAudioResource* audio = nullptr)
    {
        for(int i=0; i<(int)this->taken.size(); i++)
        {
            if(!taken[i])
            {
                return taken[i] = new pegaMixerChannel(i, this->logger, audio, this->master_volume);
            }
        }
        this->taken.resize(Mix_AllocateChannels(int(this->taken.size())*2), nullptr);
        return this->createChannel();
    }
    void deleteChannel(pegaMixerChannel* channel)
    {
        if(channel==nullptr)
            return;
        int ch = channel->channel;
        delete this->taken[ch];
        this->taken[ch] = nullptr;
    }
    friend class pegaDevice;
};

#endif

#endif
