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

#ifndef _PEGA_FRAME_H_
#define _PEGA_FRAME_H_

#include "pega_logger.h"
#include "pega_point.h"

class pegaFrame
{
protected:
    char buffer[_PEGA_MESSAGE_BUFFER_SIZE];
    pegaLogger* logger;
    pegaTextureResource* texture;
    std::string debug_name;
    int screen_width;
    int screen_height;
    pegaPoint2D position;
    double rotation;
    bool x_mirror;
    bool y_mirror;
    pegaPoint2D size;
    double radiant_upper_left;
    double radiant_upper_right;
    double radiant_lower_left;
    double radiant_lower_right;
    bool blending;
    pegaPoint2D corner_upper_left;
    pegaPoint2D corner_upper_right;
    pegaPoint2D corner_lower_left;
    pegaPoint2D corner_lower_right;
    pegaPoint2D center;
    double radius;
    pegaPoint2D texture_coords[4];
    double color_filter[4];
    ~pegaFrame()
    {
        if(this->texture)
        {
            this->texture->reference_count--;
        }
    }
    pegaFrame(pegaTextureResource* tex, pegaLogger* created_logger, std::string dName)
    {
        this->logger = created_logger;
        if(tex == nullptr && dName=="")
        {
            this->debug_name = "default_name";
        }
        else if(dName!="")
        {
            this->debug_name = dName;
        }
        else
        {
            this->debug_name = tex->debug_name;
        }
        this->texture = tex;
        if(!this->texture)
        {
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "%s has not a valid texture", this->debug_name.c_str());
            this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer);
        }
        else
        {
            this->texture->reference_count++;
            this->initInternal();
        }
    }
    void calculateRadiants()
    {
        this->radiant_upper_left = atan2(this->corner_upper_left.y-this->center.y,
                                         this->corner_upper_left.x-this->center.x);
        this->radiant_upper_right = atan2(this->corner_upper_right.y-this->center.y,
                                          this->corner_upper_right.x-this->center.x);
        this->radiant_lower_left = atan2(this->corner_lower_left.y-this->center.y,
                                         this->corner_lower_left.x-this->center.x);
        this->radiant_lower_right = atan2(this->corner_lower_right.y-this->center.y,
                                          this->corner_lower_right.x-this->center.x);
    }
    void updateRadius()
    {
        this->radius = sqrt(pow(this->center.x-this->corner_upper_left.x, 2.0)+
                            pow(this->center.y-this->corner_upper_left.y, 2.0));
    }
    void updateCorners()
    {
        this->corner_upper_left.setPoint(this->position.x, this->position.y);
        this->corner_upper_right.setPoint(this->position.x+this->size.x, this->position.y);
        this->corner_lower_left.setPoint(this->position.x, this->position.y+this->size.y);
        this->corner_lower_right.setPoint(this->position.x+this->size.x, this->position.y+this->size.y);
        this->center.setPoint((this->corner_upper_right.x+this->corner_upper_left.x)/2.0,
                              (this->corner_lower_left.y+this->corner_upper_left.y)/2.0);
    }
    void initInternal()
    {
        int screen_values[4];
        glGetIntegerv(GL_VIEWPORT, screen_values); pegaCheckGLError();
        glFinish(); pegaCheckGLError();
        this->screen_width = screen_values[2];
        this->screen_height = screen_values[3];
        this->position.setPoint(0.0, 0.0);
        this->rotation = 0.0;
        this->x_mirror = false;
        this->y_mirror = false;
        this->size.setPoint(this->texture->width, this->texture->height);
        this->blending = true;
        this->updateCorners();
        this->updateRadius();
        this->calculateRadiants();
        for(size_t i=0; i<4; i++)
            color_filter[i] = 1.0;
        // UPPER-LEFT
        this->texture_coords[0].setPoint(0.0, 0.0);
        // UPPER-RIGHT
        this->texture_coords[1].setPoint(1.0, 0.0);
        // LOWER-LEFT
        this->texture_coords[2].setPoint(0.0, 1.0);
        // LOWER-RIGHT
        this->texture_coords[3].setPoint(1.0, 1.0);
    }
    void setBlending(bool toggle)
    {
        this->blending = toggle;
    }
    void setRotation(double rad)
    {
        this->rotation = rad;
    }
    void setPosition(double x, double y)
    {
        this->position.setPoint(x, y);
        this->updateCorners();
    }
    void setCenter(double x, double y)
    {
        this->setPosition(x-this->size.x/2.0, y-this->size.y/2.0);
    }
    void resetSize()
    {
        if(!this->texture)
            return;
        this->size.x = this->texture->width;
        this->size.y = this->texture->height;
        this->updateCorners();
        this->updateRadius();
    }
    void setWidth(double w)
    {
        if(w<0)
            this->resetSize();
        else
        {
            if(!this->texture)
                return;
            this->size.y = w*double(this->texture->height)/double(this->texture->width);
            this->size.x = w;
            this->updateCorners();
            this->updateRadius();
        }
    }
    void setHeight(double h)
    {
        if(h<0)
            this->resetSize();
        else
        {
            if(!this->texture)
                return;
            this->size.x = double(this->texture->width)*h/double(this->texture->height);
            this->size.y = h;
            this->updateCorners();
            this->updateRadius();
        }
    }
    void setXMirror(bool toggle)
    {
        if(toggle!=this->x_mirror)
        {
            this->x_mirror = toggle;
            std::swap(this->texture_coords[0], this->texture_coords[1]);
            std::swap(this->texture_coords[2], this->texture_coords[3]);
        }
    }
    void setYMirror(bool toggle)
    {
        if(toggle!=this->y_mirror)
        {
            this->y_mirror = toggle;
            std::swap(this->texture_coords[0], this->texture_coords[2]);
            std::swap(this->texture_coords[1], this->texture_coords[3]);
        }
    }
    double clamp01(double num)
    {
        if(num<0.0)
            return 0.0;
        if(num>1.0)
            return 1.0;
        return num;
    }
    void setColorFilter(double red, double green, double blue, double alpha = 1.0)
    {
        this->color_filter[0] = this->clamp01(red);
        this->color_filter[1] = this->clamp01(green);
        this->color_filter[2] = this->clamp01(blue);
        this->color_filter[3] = this->clamp01(alpha);
    }
    void draw()
    {
        if(!this->texture)
            return;
        if(this->blending)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        else
        {
            glDisable(GL_BLEND);
        }
        glMatrixMode(GL_PROJECTION); pegaCheckGLError();
        glPushMatrix(); pegaCheckGLError();
        glLoadIdentity(); pegaCheckGLError();
        glMatrixMode(GL_MODELVIEW); pegaCheckGLError();
        glPushMatrix(); pegaCheckGLError();
        glLoadIdentity(); pegaCheckGLError();
        gluOrtho2D(0.0, double(this->screen_width), double(this->screen_height), 0.0); pegaCheckGLError();
        glBindTexture(GL_TEXTURE_2D, this->texture->texture); pegaCheckGLError();
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); pegaCheckGLError();
        glColor4dv(this->color_filter);
        glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2d(this->texture_coords[0].x, this->texture_coords[0].y);
            glVertex2d(this->center.x+this->radius*cos(this->rotation+this->radiant_upper_left),
                       this->center.y+this->radius*sin(this->rotation+this->radiant_upper_left));
            glTexCoord2d(this->texture_coords[1].x, this->texture_coords[1].y);
            glVertex2d(this->center.x+this->radius*cos(this->rotation+this->radiant_upper_right),
                       this->center.y+this->radius*sin(this->rotation+this->radiant_upper_right));
            glTexCoord2d(this->texture_coords[2].x, this->texture_coords[2].y);
            glVertex2d(this->center.x+this->radius*cos(this->rotation+this->radiant_lower_left),
                       this->center.y+this->radius*sin(this->rotation+this->radiant_lower_left));
            glTexCoord2d(this->texture_coords[3].x, this->texture_coords[3].y);
            glVertex2d(this->center.x+this->radius*cos(this->rotation+this->radiant_lower_right),
                       this->center.y+this->radius*sin(this->rotation+this->radiant_lower_right));
        glEnd(); pegaCheckGLError();
        glBindTexture(GL_TEXTURE_2D, 0); pegaCheckGLError();
        glPopMatrix(); pegaCheckGLError();
        glMatrixMode(GL_PROJECTION); pegaCheckGLError();
        glPopMatrix(); pegaCheckGLError();
        glMatrixMode(GL_MODELVIEW); pegaCheckGLError();
    }
public:
    friend class pegaAnimation;
};

#endif

#endif
