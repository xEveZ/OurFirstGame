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

#ifndef _PEGA_LOGGER_H_
#define _PEGA_LOGGER_H_

#define PEGA_LOGGER_DEBUG 0
#define PEGA_LOGGER_INFORMATION 1
#define PEGA_LOGGER_WARNING 2
#define PEGA_LOGGER_ERROR 3
#define PEGA_LOGGER_CRITICAL 4
#define _PEGA_LOGGER_NUMBER_OF_STREAMS 5
#define PEGA_LOGGER_NONE -1

class pegaLogger
{
protected:
    char buffer[_PEGA_MESSAGE_BUFFER_SIZE];
    FILE* stream[_PEGA_LOGGER_NUMBER_OF_STREAMS];
    int minimum_streamable;
    bool isValidStream(int stream_level)
    {
        if(stream_level<0 || stream_level>=_PEGA_LOGGER_NUMBER_OF_STREAMS)
            return false;
        return true;
    }
public:
    pegaLogger(int verbosity = PEGA_LOGGER_INFORMATION,
               FILE* debug_stream = stdout,
               FILE* information_stream = stdout,
               FILE* warning_stream = stderr,
               FILE* error_stream = stderr,
               FILE* critical_stream = stderr)
    {
        this->minimum_streamable = verbosity;
        this->stream[PEGA_LOGGER_DEBUG] = debug_stream;
        this->stream[PEGA_LOGGER_INFORMATION] = information_stream;
        this->stream[PEGA_LOGGER_WARNING] = warning_stream;
        this->stream[PEGA_LOGGER_ERROR] = error_stream;
        this->stream[PEGA_LOGGER_CRITICAL] = critical_stream;
    } ///< PEGA_LOGGER_DEBUG, PEGA_LOGGER_INFORMATION, PEGA_LOGGER_WARNING, PEGA_LOGGER_ERROR, PEGA_LOGGER_CRITICAL or PEGA_LOGGER_NONE
    void setVerbosity(int stream_level)
    {
        if(!this->isValidStream(stream_level))
            stream_level = PEGA_LOGGER_NONE;
        this->minimum_streamable = stream_level;
        return;
    } ///< PEGA_LOGGER_DEBUG, PEGA_LOGGER_INFORMATION, PEGA_LOGGER_WARNING, PEGA_LOGGER_ERROR, PEGA_LOGGER_CRITICAL or PEGA_LOGGER_NONE
    void setStream(int stream_level, FILE* stream_channel)
    {
        if(!this->isValidStream(stream_level))
            return;
        this->stream[stream_level] = stream_channel;
        return;
    } ///< PEGA_LOGGER_DEBUG, PEGA_LOGGER_INFORMATION, PEGA_LOGGER_WARNING, PEGA_LOGGER_ERROR, PEGA_LOGGER_CRITICAL or PEGA_LOGGER_NONE
    void writeMessage(int stream_level, const char* message)
    {
        if(!this->isValidStream(stream_level))
            return;
        if(stream_level<this->minimum_streamable)
            return;
        char temp_buffer[_PEGA_MESSAGE_BUFFER_SIZE];
        memset(this->buffer, 0, sizeof(char)*_PEGA_MESSAGE_BUFFER_SIZE);
        switch(stream_level)
        {
            case PEGA_LOGGER_INFORMATION:
                snprintf(temp_buffer, _PEGA_MESSAGE_BUFFER_SIZE, "INFO: ");
            break;

            case PEGA_LOGGER_WARNING:
                snprintf(temp_buffer, _PEGA_MESSAGE_BUFFER_SIZE, "WARNING: ");
            break;

            case PEGA_LOGGER_ERROR:
                snprintf(temp_buffer, _PEGA_MESSAGE_BUFFER_SIZE, "ERROR: ");
            break;

            case PEGA_LOGGER_CRITICAL:
                snprintf(temp_buffer, _PEGA_MESSAGE_BUFFER_SIZE, "CRITICAL: ");
            break;

            case PEGA_LOGGER_DEBUG:
                snprintf(temp_buffer, _PEGA_MESSAGE_BUFFER_SIZE, "DEBUG: ");
            break;

            default:
                return;
            break;
        }
        strcat(this->buffer, temp_buffer);
        snprintf(temp_buffer, _PEGA_MESSAGE_BUFFER_SIZE-64, "%s\n", message);
        strcat(this->buffer, temp_buffer);
        fprintf(this->stream[stream_level], "%s", this->buffer);
        fflush(this->stream[stream_level]);
        return;
    } ///< PEGA_LOGGER_DEBUG, PEGA_LOGGER_INFORMATION, PEGA_LOGGER_WARNING, PEGA_LOGGER_ERROR, PEGA_LOGGER_CRITICAL or PEGA_LOGGER_NONE
};

#endif

#endif
