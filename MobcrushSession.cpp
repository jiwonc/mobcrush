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
#include "MobcrushSession.h"
#include <videocore/rtmp/RTMPSession.h>


#include <videocore/transforms/RTMP/AACPacketizer.h>
#include <videocore/transforms/RTMP/H264Packetizer.h>
#include <videocore/transforms/Split.h>

#ifdef __APPLE__
#   include <videocore/mixers/Apple/AudioMixer.h>
#   ifdef TARGET_OS_IPHONE
#       include <videocore/sources/iOS/AudioUnitSource.h>
#       include <videocore/sources/iOS/GLESSource.h>
#       include <videocore/sources/iOS/CameraSource.h>
#       include <videocore/sources/iOS/MicSource.h>
#       include <videocore/mixers/iOS/GLESVideoMixer.h>
#       include <videocore/transforms/iOS/AACEncode.h>
#       include <videocore/transforms/iOS/H264Encode.h>

#   else /* OS X */

#   endif
#else
#   include <videocore/mixers/GenericAudioMixer.h>
#endif

namespace mobcrush
{
    MobcrushSession::MobcrushSession(SessionStateCallback_t callback)
    : m_callback(callback), m_micEnabled(false), m_micGain(1.f), m_gameGain(1.f)
    {
        
    }
    
    MobcrushSession::~MobcrushSession()
    {
        std::cout << "MobcrushSession::~MobcrushSession()" << std::endl;
    }
    
    void
    MobcrushSession::startRtmpSession(std::string uri, int frame_w, int frame_h, int bitrate, int fps)
    {
        std::cout << "This is an evaluation version of Mobcrush.  It is for evaluation purposes only." << std::endl;
        std::cout << "Starting RTMP Session with " << uri << " ... " << std::endl;
        m_outputSession.reset( new videocore::RTMPSession ( uri, [=](videocore::RTMPSession& session, ClientState_t state) {
            
            switch(state) {
                    
                case kClientStateSessionStarted:
                {
                    std::cout << "RTMP Started\n";
                    this->m_state = kSessionStateStarted;
                    
                    this->setupGraph(frame_w, frame_h, fps, bitrate);

                    this->m_callback(kSessionStateStarted) ;
                }
                    break;
                case kClientStateError:
                    std::cout << "RTMP Error\n";
                    m_state = kSessionStateError;
                    this->m_callback(kSessionStateError);
                    break;
                case kClientStateNotConnected:
                    std::cout << "RTMP Ended\n";
                    m_state = kSessionStateEnded;
                    this->m_callback(kSessionStateEnded);
                    break;
                default:
                    break;
                    
            }
            
        }) );
        videocore::RTMPSessionParameters_t sp ( 0. );
        
        sp.setData(frame_w, frame_h, 1. / static_cast<double>(fps), bitrate);
        
        m_outputSession->setSessionParameters(sp);
        
    }

    void
    MobcrushSession::addTransform(std::vector<std::shared_ptr<videocore::ITransform> > &chain, std::shared_ptr<videocore::ITransform> transform)
    {
        if( chain.size() > 0 ) {
            chain.back()->setOutput(transform);
        }
        chain.push_back(transform);
        
    }
    void
    MobcrushSession::setupGraph( int frame_w, int frame_h, int fps, int bitrate)
    {

        m_audioTransformChain.clear();
        m_videoTransformChain.clear();
        

        {
            const double aacPacketTime = 1024. / 44100.0;
            const double bufferTime = std::nearbyint(0.033333  / aacPacketTime) * aacPacketTime;

//#ifdef __APPLE__
//            addTransform(m_audioTransformChain, std::make_shared<videocore::Apple::AudioMixer>(2,44100,16,bufferTime));
//#else
            addTransform(m_audioTransformChain, std::make_shared<videocore::GenericAudioMixer>(2,44100,16,bufferTime));
//#endif
        }
#ifdef __APPLE__
#ifdef TARGET_OS_IPHONE


        {
            videocore::iOS::AudioUnitSource::setStaticOutput(m_audioTransformChain.back());
        }
        {
            auto mixer = std::make_shared<videocore::iOS::GLESVideoMixer>(frame_w, frame_h, 1. / static_cast<double>(fps));
            videocore::iOS::GLESSource::setStaticOutput(mixer);
            addTransform(m_videoTransformChain, mixer);
           
        }
        {
            // Add encoders
            addTransform(m_audioTransformChain, std::make_shared<videocore::iOS::AACEncode>(44100, 2));
            addTransform(m_videoTransformChain, std::make_shared<videocore::iOS::H264Encode>(frame_w, frame_h, fps, bitrate));
            
        }
        {
            // Add splits
            auto videoSplit = std::make_shared<videocore::Split>();
            auto audioSplit = std::make_shared<videocore::Split>();
            m_videoSplit = videoSplit;
            m_audioSplit = audioSplit;
            
            addTransform(m_audioTransformChain, audioSplit);
            addTransform(m_videoTransformChain, videoSplit);
            
        }
#else
#endif // TARGET_OS_IPHONE
#endif // __APPLE__
        
        addTransform(m_audioTransformChain, std::make_shared<videocore::rtmp::AACPacketizer>());
        addTransform(m_videoTransformChain, std::make_shared<videocore::rtmp::H264Packetizer>());
        

        m_videoTransformChain.back()->setOutput(this->m_outputSession);
        m_audioTransformChain.back()->setOutput(this->m_outputSession);
        
        const auto epoch = std::chrono::steady_clock::now();
        for(auto it = m_videoTransformChain.begin() ; it != m_videoTransformChain.end() ; ++it) {
            (*it)->setEpoch(epoch);
        }
        for(auto it = m_audioTransformChain.begin() ; it != m_audioTransformChain.end() ; ++it) {
            (*it)->setEpoch(epoch);
        }
        
        if(m_micEnabled) {
            microphoneEnabled(true);
        }
        
    }
    void
    MobcrushSession::cameraEnable(float x, float y, float w, float h, float aspect)
    {
        if(m_state == kSessionStateStarted) {

            m_cameraSource = std::make_shared<videocore::iOS::CameraSource>(x,y,w,h, aspect);
            m_cameraSource->setOutput(m_videoTransformChain.front());

        }
    }
    void
    MobcrushSession::cameraDisable()
    {
        m_cameraSource.reset();
    }
    void
    MobcrushSession::microphoneEnabled(bool enabled)
    {
        m_micEnabled = enabled;
        if(enabled && m_state == kSessionStateStarted) {
            m_micSource = std::make_shared<videocore::iOS::MicSource>();
            m_micSource->setOutput(m_audioTransformChain.front());
            auto mixer = std::dynamic_pointer_cast<videocore::IAudioMixer>(m_audioTransformChain.front());
            mixer->setSourceGain(m_micSource, m_micGain);
            
        } else {
            m_micSource.reset();
        }
    }
    void
    MobcrushSession::microphoneGain(float gain)
    {
        m_micGain = gain;
        if(m_micEnabled && m_state == kSessionStateStarted) {
            auto mixer = std::dynamic_pointer_cast<videocore::IAudioMixer>(m_audioTransformChain.front());
            mixer->setSourceGain(m_micSource, gain);
        }
    }
    void
    MobcrushSession::gameGain(float gain)
    {
        m_gameGain = gain;
        
#ifdef __APPLE__
#ifdef TARGET_OS_IPHONE
      
        videocore::iOS::AudioUnitSource::setStaticGain(gain);
        
#endif
#endif
    }
}