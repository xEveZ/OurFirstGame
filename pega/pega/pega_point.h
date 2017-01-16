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

#ifndef _PEGA_POINT_H_
#define _PEGA_POINT_H_

class pegaPoint2D
{
public:
    double x;
    double y;
    pegaPoint2D(double x0 = 0.0, double y0 = 0.0)
    {
        this->x = x0;
        this->y = y0;
    }
    pegaPoint2D(const pegaPoint2D& p)
    {
        this->x = p.x;
        this->y = p.y;
    }
    void setPoint(double x0 = 0.0, double y0 = 0.0)
    {
        this->x = x0;
        this->y = y0;
    }
};

class pegaPoint3D
{
public:
    double x;
    double y;
    double z;
    pegaPoint3D(double x0 = 0.0, double y0 = 0.0, double z0 = 0.0)
    {
        this->x = x0;
        this->y = y0;
        this->z = z0;
    }
    pegaPoint3D(const pegaPoint3D& p)
    {
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
    }
    void setPoint(double x0 = 0.0, double y0 = 0.0, double z0 = 0.0)
    {
        this->x = x0;
        this->y = y0;
        this->z = z0;
    }
};

#endif

#endif
