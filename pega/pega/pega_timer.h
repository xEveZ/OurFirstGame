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

#ifndef _PEGA_TIMER_H_
#define _PEGA_TIMER_H_

class pegaTimer
{
protected:
    std::chrono::high_resolution_clock::time_point start_time;
    long long int offsetMicro;
    bool paused;
public:
    void reset()
    {
        this->start_time = std::chrono::high_resolution_clock::now();
        this->offsetMicro = 0LL;
        this->paused = false;
    }
    void pause()
    {
        this->offsetMicro = this->getMicroSeconds();
        this->paused = true;
    }
    void resume()
    {
        if(!this->paused)
            return;
        this->start_time = std::chrono::high_resolution_clock::now();
        this->paused = false;
    }
    pegaTimer()
    {
        this->reset();
    }
    long long int getMicroSeconds()
    {
        if(this->paused)
            return this->offsetMicro;
        std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<long long int, std::micro> time_passed = std::chrono::duration_cast<std::chrono::duration<long long int, std::micro> >(end_time-this->start_time);
        return time_passed.count() + this->offsetMicro;
    }
    long long int getMilliSeconds()
    {
        if(this->paused)
            return this->offsetMicro/1000LL;
        std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<long long int, std::milli> time_passed = std::chrono::duration_cast<std::chrono::duration<long long int, std::milli> >(end_time-this->start_time);
        return time_passed.count() + (this->offsetMicro/1000LL);
    }
};

#endif

#endif
