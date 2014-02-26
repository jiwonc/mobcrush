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
#ifndef __mobcrush__MobcrushSession__
#define __mobcrush__MobcrushSession__

#include <iostream>
#include <videocore/transforms/IOutputSession.hpp>
#include <videocore/transforms/ITransform.hpp>
#include <videocore/sources/ISource.hpp>
#include <videocore/mixers/IMixer.hpp>

#include <vector>
#include <functional>

namespace mobcrush
{
    typedef enum
    {
        kSessionStateNone,
        kSessionStateStarted,
        kSessionStateEnded,
        kSessionStateError
        
    } SessionState_t ;
    
    typedef std::function<void(SessionState_t sessionState)> SessionStateCallback_t;
    
    class MobcrushSession
    {
    public:
        
        MobcrushSession(SessionStateCallback_t callback);
        ~MobcrushSession();
        
    public:
        // Starting a new session will end the current session.
        void startRtmpSession(std::string uri, int frame_w, int frame_h, int bitrate, int fps);
        
        void cameraEnable(float x, float y, float w, float h, float aspect);
        void cameraDisable();
        
        void microphoneEnabled(bool enabled);
        void microphoneGain(float gain);
        void gameGain(float gain);
        
    private:
        
        void setupGraph(int frame_w, int frame_h, int fps, int bitrate);
        void addTransform(std::vector< std::shared_ptr<videocore::ITransform> > & chain, std::shared_ptr<videocore::ITransform> transform);
        
    private:
        
        
        std::shared_ptr<videocore::ISource> m_micSource;
        std::shared_ptr<videocore::ISource> m_cameraSource;
        
        std::vector< std::shared_ptr<videocore::ITransform> > m_audioTransformChain;
        std::vector< std::shared_ptr<videocore::ITransform> > m_videoTransformChain;
        
        std::weak_ptr<videocore::ITransform> m_audioSplit;
        std::weak_ptr<videocore::ITransform> m_videoSplit;
        
        std::shared_ptr<videocore::IOutputSession> m_outputSession;
        
        SessionState_t         m_state;
        
        SessionStateCallback_t m_callback;
        
        float m_micGain;
        float m_gameGain;
        
        bool m_micEnabled;
    };
}

#endif /* defined(__mobcrush__MobcrushSession__) */
