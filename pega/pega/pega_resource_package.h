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

#ifndef _PEGA_RESOURCE_PACKAGE_H_
#define _PEGA_RESOURCE_PACKAGE_H_

#include "pega_logger.h"
#include "pega_texture_resource.h"
#include "pega_audio_resource.h"
#include "pega_font_resource.h"

class pegaResourcePackage
{
protected:
    char buffer[_PEGA_MESSAGE_BUFFER_SIZE];
    pegaLogger* logger;
    std::string debug_name;
    unsigned long long int id;
    zip* zip_archive;
    pegaResourcePackage(zip* archive, std::string dName, pegaLogger* created_logger, unsigned long long int my_id)
    {
        this->logger = created_logger;
        this->zip_archive = archive;
        this->debug_name = dName;
        this->id = my_id;
    }
    bool getRAWData(const char* file_name, std::vector<unsigned char>& v)
    {
        v.clear();
        zip_file* file_zip = zip_fopen(this->zip_archive, file_name, 0);
        if(!file_zip)
        {
            return false;
        }
        const size_t CHUNK_SIZE = 512UL;
        unsigned char temp_buffer[CHUNK_SIZE];
        long long int read_this_time;
        while((read_this_time = zip_fread(file_zip, temp_buffer, CHUNK_SIZE)))
        {
            if(read_this_time < 0)
            {
                zip_fclose(file_zip);
                v.clear();
                return false;
            }
            v.insert(v.end(), temp_buffer, temp_buffer+read_this_time);
        }
        zip_fclose(file_zip);
        return true;
    }
public:
    void setArchivePassword(bool toggle, const char* password = nullptr)
    {
        if(!toggle)
        {
            zip_set_default_password(this->zip_archive, nullptr);
        }
        else
        {
            zip_set_default_password(this->zip_archive, password);
        }
    }
    friend class pegaResourceManager;
};

#endif

#endif
