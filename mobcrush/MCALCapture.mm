/*
 
 Mobcrush
 Copyright (C) 2014 James G. Hurley
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 USA
 
 */
#import <OpenAL/al.h>
#import "MCALCapture.h"
#include <dlfcn.h>
#include <map>
#include <vector>
/*
 alBufferData
 alSourcei
 alSourcef
 alSourcePlay
 alSourceStop
 alDeleteBuffers
 alcMakeContextCurrent
*/
extern void *(*g_dls)(void*, const char*);

typedef struct {
    std::vector<char> data;
    ALenum format;
    ALsizei frequency;
} BufferData_t;
static std::map<ALuint, BufferData_t > s_bufferMap;

extern "C" {
    // format: AL_FORMAT_MONO8, MONO16, STEREO8, STEREO16
    void alBufferData(ALuint bufferName, ALenum format, const ALvoid* data, ALsizei size, ALsizei frequency) {
        typedef void (*albd)(ALuint, ALenum, const ALvoid*, ALsizei, ALsizei);
        static albd alBufferData_IMP = NULL;
        if(!alBufferData_IMP) alBufferData_IMP = (albd)g_dls(RTLD_NEXT, "alBufferData");
        
        s_bufferMap[bufferName].data.resize(size);
        memcpy(&s_bufferMap[bufferName].data[0], data, size);
        s_bufferMap[bufferName].format = format;
        s_bufferMap[bufferName].frequency = frequency;
        
        return alBufferData_IMP(bufferName, format, data, size, frequency);
    }
    
    void alSourcef(ALuint source, ALenum param, ALfloat value) {
        typedef void (*alsf)(ALuint, ALenum, ALfloat);
        static alsf alSourcef_IMP = NULL;
        if(!alSourcef_IMP) alSourcef_IMP = (alsf)g_dls(RTLD_NEXT, "alSourcef");
        
        return alSourcef_IMP(source,param,value);
    }
    
    void alSourcei(ALuint source, ALenum param, ALint value) {
        
    }
}
@implementation MCALCapture

@end
