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

#ifndef _PEGA_MIXER_CHANNEL_H_
#define _PEGA_MIXER_CHANNEL_H_

#include "pega_logger.h"
#include "pega_audio_resource.h"

class pegaMixerChannel
{
protected:
    char buffer[_PEGA_MESSAGE_BUFFER_SIZE];
    pegaLogger* logger;
    std::string debug_name;
    int channel;
    double master_volume;
    double local_volume;
    pegaAudioResource* audio;
    pegaMixerChannel(int ch, pegaLogger* created_logger, pegaAudioResource* audio_res, double mv)
    {
        this->channel = ch;
        Mix_Volume(this->channel, MIX_MAX_VOLUME);
        this->logger = created_logger;
        this->debug_name = "default_name";
        this->audio = audio_res;
        this->master_volume = mv;
        this->local_volume = 1.0;
        if(this->audio)
        {
            this->audio->reference_count++;
        }
    }
    ~pegaMixerChannel()
    {
        Mix_HaltChannel(this->channel);
        if(this->audio)
        {
            this->audio->reference_count--;
        }
    }
    void calcVolume()
    {
        Mix_Volume(this->channel, int(this->local_volume*this->master_volume*double(MIX_MAX_VOLUME)));
    }
public:
    void setAudio(pegaAudioResource* audio_resource)
    {
        if(this->audio)
        {
            this->stop();
            this->audio->reference_count--;
        }
        this->audio = audio_resource;
        if(this->audio)
        {
            this->audio->reference_count++;
        }
    }
    void setDebugName(std::string dName)
    {
        this->debug_name = dName;
    }
    void setVolume(double volume)
    {
        if(volume < 0.0)
            volume = 0.0;
        if(volume > 1.0)
            volume = 1.0;
        this->local_volume = volume;
    }
    void stopWithFadeOut(int msFadeOut)
    {
        if(!Mix_Playing(this->channel))
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Channel %d ( %s ) is not playing", this->channel, this->debug_name.c_str());
            this->logger->writeMessage(PEGA_LOGGER_WARNING, this->buffer);
            return;
        }
        Mix_FadeOutChannel(this->channel, msFadeOut);
    }
    void stop()
    {
        Mix_HaltChannel(this->channel);
    }
    void resume()
    {
        if(!Mix_Paused(this->channel))
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Channel %d ( %s ) is not paused", this->channel, this->debug_name.c_str());
            this->logger->writeMessage(PEGA_LOGGER_WARNING, this->buffer);
            return;
        }
        Mix_Resume(this->channel);
    }
    void pause()
    {
        if(!Mix_Playing(this->channel))
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Channel %d ( %s ) is not playing", this->channel, this->debug_name.c_str());
            this->logger->writeMessage(PEGA_LOGGER_WARNING, this->buffer);
            return;
        }
        Mix_Pause(this->channel);
    }
    void play(bool loop = false, int msFadeIn = -1, int msTimed = -1)
    {
        if(this->audio==nullptr)
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "Channel %d ( %s ) has no audio set", this->channel, this->debug_name.c_str());
            this->logger->writeMessage(PEGA_LOGGER_WARNING, this->buffer);
            return;
        }
        if(Mix_Playing(this->channel) || Mix_Paused(this->channel))
        {
            Mix_HaltChannel(this->channel);
        }
        int loops = 0;
        if(loop)
            loops = -1;
        if(msFadeIn>=0 && msTimed>=0)
        {
            Mix_FadeInChannelTimed(this->channel, this->audio->audio_data, loops, msFadeIn, msTimed);
        }
        else if(msFadeIn>=0)
        {
            Mix_FadeInChannel(this->channel, this->audio->audio_data, loops, msFadeIn);
        }
        else if(msTimed>=0)
        {
            Mix_PlayChannelTimed(this->channel, this->audio->audio_data, loops, msTimed);
        }
        else
        {
            Mix_PlayChannel(this->channel, this->audio->audio_data, loops);
        }
    }
    bool isPlaying()
    {
        return bool(Mix_Playing(this->channel));
    }
    bool isPaused()
    {
        return bool(Mix_Paused(this->channel));
    }
    friend class pegaMixer;
};

#endif

#endif
