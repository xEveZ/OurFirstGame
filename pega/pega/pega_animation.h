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

#ifndef _PEGA_ANIMATION_H_
#define _PEGA_ANIMATION_H_

#include "pega_logger.h"
#include "pega_frame.h"
#include "pega_timer.h"

class pegaAnimation
{
protected:
    char buffer[_PEGA_MESSAGE_BUFFER_SIZE];
    pegaLogger* logger;
    std::string debug_name;
    unsigned long long int id;
    std::vector<pegaFrame*> frames;
    pegaTimer* internal_timer;
    double fps;
    bool absolute_position;
    double x_position;
    double y_position;
    bool active_width;
    double width;
    double height;
    double rotation;
    bool x_mirror;
    bool y_mirror;
    bool loop;
    double color_filter[4];
    bool blending;
    pegaAnimation(pegaLogger* created_logger, std::string dName, unsigned long long int my_id)
    {
        this->logger = created_logger;
        this->debug_name = dName;
        this->id = my_id;
        this->internal_timer = new pegaTimer();
        this->fps = 1.0;
        this->absolute_position = true;
        this->x_position = 0.0;
        this->y_position = 0.0;
        this->active_width = true;
        this->width = -1.0;
        this->height = -1.0;
        this->rotation = 0.0;
        this->x_mirror = false;
        this->y_mirror = false;
        this->loop = true;
        this->blending = true;
        for(size_t i=0; i<4; i++)
        {
            this->color_filter[i] = 1.0;
        }
    }
    ~pegaAnimation()
    {
        delete this->internal_timer;
        for(size_t i=0; i<this->frames.size(); i++)
        {
            delete frames[i];
        }
        this->frames.clear();
    }
    int calculateFrame()
    {
        if(this->frames.size()==0)
            return -1;
        double time_passed = double(this->internal_timer->getMicroSeconds());
        double time_per_frame = 1000000.0/this->fps;
        int actual_frame = int(time_passed/time_per_frame);
        if(!this->loop)
        {
            if(actual_frame>=(int)this->frames.size())
            {
                return -1;
            }
        }
        actual_frame %= this->frames.size();
        return actual_frame;
    }
public:
    void setBlending(bool toggle)
    {
        this->blending = toggle;
    }
    void draw()
    {
        int frame = this->calculateFrame();
        if(frame<0)
            return;
        if(this->active_width)
            this->frames[frame]->setWidth(this->width);
        else
            this->frames[frame]->setHeight(this->height);
        if(this->absolute_position)
            this->frames[frame]->setPosition(this->x_position, this->y_position);
        else
            this->frames[frame]->setCenter(this->x_position, this->y_position);
        this->frames[frame]->setRotation(this->rotation);
        this->frames[frame]->setXMirror(this->x_mirror);
        this->frames[frame]->setYMirror(this->y_mirror);
        this->frames[frame]->setBlending(this->blending);
        this->frames[frame]->setColorFilter(this->color_filter[0], this->color_filter[1], this->color_filter[2], this->color_filter[3]);
        this->frames[frame]->draw();
    }
    void setLoop(bool toggle)
    {
        this->loop = toggle;
    }
    void setCenter(double x, double y)
    {
        this->absolute_position = false;
        this->x_position = x;
        this->y_position = y;
    }
    void setPosition(double x, double y)
    {
        this->absolute_position = true;
        this->x_position = x;
        this->y_position = y;
    }
    void setYMirror(bool toggle)
    {
        this->y_mirror = toggle;
    }
    void setXMirror(bool toggle)
    {
        this->x_mirror = toggle;
    }
    void setRotation(double rad)
    {
        this->rotation = rad;
    }
    void setHeight(double h)
    {
        this->active_width = false;
        this->height = h;
    }
    void setWidth(double w)
    {
        this->active_width = true;
        this->width = w;
    }
    void setFPS(double frame_per_second)
    {
        if(frame_per_second<=0.0)
            this->fps = 1.0;
        else
            this->fps = frame_per_second;
    }
    void reset()
    {
        this->internal_timer->reset();
    }
    void pause()
    {
        this->internal_timer->pause();
    }
    void resume()
    {
        this->internal_timer->resume();
    }
    void pushFrame(pegaTextureResource* texture)
    {
        this->frames.push_back(new pegaFrame(texture, this->logger, this->debug_name));
    }
    void popFrame()
    {
        if(this->frames.size()==0)
            return;
        delete this->frames[this->frames.size()-1];
        this->frames.pop_back();
    }
    void setColorFilter(double red, double green, double blue, double alpha = 1.0)
    {
        this->color_filter[0] = red;
        this->color_filter[1] = green;
        this->color_filter[2] = blue;
        this->color_filter[3] = alpha;
    }
    friend class pegaAnimationManager;
};

#endif

#endif
