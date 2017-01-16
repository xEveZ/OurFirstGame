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

#ifndef _PEGA_H_
#define _PEGA_H_

#ifndef _MSC_VER
#if ((!defined __cplusplus) || (__cplusplus<201100L))
#error C++11 is not supported
#endif
#endif

#define PEGA_VERSION_MAJOR 0
#define PEGA_VERSION_MINOR 3
#define PEGA_VERSION_PATCH 1
#define PEGA_VERSION_OTHER "alpha"

#ifdef PEGA_DEBUG
#define _PEGA_DEBUG true
#else
#define _PEGA_DEBUG false
#endif

#if defined _WIN32
#define PEGA_OS_WINDOWS
#elif defined __linux__
#define PEGA_OS_LINUX
#elif defined __APPLE__
#define PEGA_OS_MACOSX
#elif defined __FreeBSD__
#define PEGA_OS_FREEBSD
#elif defined __unix__
#define PEGA_OS_OTHERUNIX
#else
#define PEGA_OS_UNKNOWN
#endif

#define _PEGA_MESSAGE_BUFFER_SIZE 4096

// STANDARD INCLUDES

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdarg>
#include <new>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cctype>
#include <random>
#include <cassert>
#include <cstring>
#include <cstdint>
#include <cstddef>
#include <cmath>
#include <ciso646>
#include <ctime>
#include <set>
#include <queue>
#include <deque>
#include <map>
#include <climits>
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

// PATCH

#ifdef PEGA_OS_WINDOWS
#include <winsock2.h>
#define SDL_MAIN_HANDLED
#define socklen_t int
#else
#define SOCKET int
#endif

// EXTERNAL LIBRARIES INCLUDES

#ifdef PEGA_OS_WINDOWS
#define GLEW_STATIC
#endif
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#ifdef PEGA_OS_MACOSX
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include <zip.h>

// OpenGL DEBUG

#define pegaCheckGLError() \
if(_PEGA_DEBUG) \
{ \
    GLenum err = GL_NO_ERROR; \
    do \
    { \
        err = glGetError(); \
        if(err!=GL_NO_ERROR) \
        { \
            snprintf(this->buffer, _PEGA_MESSAGE_BUFFER_SIZE, "%s:%d -> %s", __FILE__, __LINE__, (const char*)gluErrorString(err)); \
            this->logger->writeMessage(PEGA_LOGGER_ERROR, this->buffer); \
        } \
    }while(err!=GL_NO_ERROR); \
}

// PEGASUS ENGINE LIBRARY INCLUDES

#include "pega/pega_device.h"
#include "pega/pega_logger.h"
#include "pega/pega_window.h"
#include "pega/pega_event.h"
#include "pega/pega_timer.h"
#include "pega/pega_resource_manager.h"
#include "pega/pega_resource_package.h"
#include "pega/pega_texture_resource.h"
#include "pega/pega_audio_resource.h"
#include "pega/pega_font_resource.h"
#include "pega/pega_event_keyboard.h"
#include "pega/pega_event_mouse.h"
#include "pega/pega_mixer.h"
#include "pega/pega_mixer_channel.h"
#include "pega/pega_frame.h"
#include "pega/pega_animation.h"
#include "pega/pega_animation_manager.h"
#include "pega/pega_font_renderer.h"
#include "pega/pega_event_joystick.h"

#undef pegaCheckGLError

#endif
