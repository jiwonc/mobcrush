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
#import <MobCrush/MobCrush.h>
#import <AudioToolbox/AudioToolbox.h>
#import <OpenAL/al.h>
#include <dlfcn.h>
#import "MCPvt.h"

#include "MobCrush_c.h"

#include <videocore/sources/iOS/GLESSource.h>


const NSString* MCVideoBitrateKey = @"MCVideoBitrateKey";
const NSString* MCVideoFramerateKey = @"MCVideoFramerateKey";
const NSString* MCVideoWidthKey = @"MCVideoWidthKey";
const NSString* MCVideoHeightKey = @"MCVideoHeightKey";

const NSString* MCRTMPServerURIKey = @"MCRTMPServerURIKey";
const NSString* MCRTMPServerStreamKey = @"MCRTMPServerStreamKey";

const NSString* MCConnectionStateChangedNotification = @"MCConnectionStateChangedNotification";

static MobCrush* s_mobCrush = NULL;


static mobcrush_connection_callback_t s_callback = NULL;

extern "C"{
    
    int
    mobcrush_connect(const char* server_uri, const char* stream_key, int bitrate, int fps, int width, int height, mobcrush_connection_callback_t callback) {
        
        NSError* error;
        return [MobCrush connectWithSettings:@{MCRTMPServerURIKey: [NSString stringWithUTF8String: server_uri],
                                    MCRTMPServerStreamKey: [NSString stringWithUTF8String: stream_key],
                                    MCVideoBitrateKey: @(bitrate),
                                    MCVideoFramerateKey: @(fps),
                                    MCVideoWidthKey: @(width),
                                    MCVideoHeightKey: @(height)} error:&error];
        s_callback = callback;
    }
    void mobcrush_disconnect() {
        [MobCrush disconnect];
    }
    void mobcrush_enable_mic() {
        [MobCrush enableMicCapture];
    }
    void mobcrush_disable_mic() {
        [MobCrush disableMicCapture];
    }
    void mobcrush_enable_camera(float x, float y, float w, float h)
    {
        [MobCrush enableCameraCaptureWithRect:CGRectMake(x, y, w, h)];
    }
    void mobcrush_disable_camera()
    {
        [MobCrush disableCameraCapture];
    }
    void mobcrush_mic_gain(float gain) {
        [MobCrush setMicrophoneGain:gain];
    }
    void mobcrush_game_gain(float gain) {
        [MobCrush setGameGain:gain];
    }
    void mobcrush_exclude_gles_context(void* context) {
        [MobCrush excludeEAGLContextFromCapture:(EAGLContext*)context];
    }
}

std::string g_tmpFolder;

@implementation MobCrush
+ (void) initialize
{
    static BOOL initialized = NO;
    if(!initialized)
    {
        initialized = YES;
        
        s_mobCrush = [[MobCrush alloc] init];
    }
}

+ (MobCrush*) sharedInstance
{
    return s_mobCrush;
}

+ (void) start {
    //[MCGLContext start];
    
}

+ (BOOL) connectWithSettings: (NSDictionary*) settings error: (NSError**) error;
{
    g_tmpFolder = [NSTemporaryDirectory() UTF8String];
    
    return [[MobCrush sharedInstance] connectWithSettings:settings error:error];
}
+ (void) disconnect
{
    [[MobCrush sharedInstance] disconnect];
}
+ (void) excludeEAGLContextFromCapture:(EAGLContext *)context {
    videocore::iOS::GLESSource::excludeFromCapture(context);
}
+ (void) enableCameraCaptureWithRect: (CGRect) location
{
    [[MobCrush sharedInstance] enableCameraCaptureWithRect:location];
}
+ (void) disableCameraCapture
{
    [[MobCrush sharedInstance] disableCameraCapture];
}
+ (void) showCameraPreview
{
    
}
+ (void) hideCameraPreview
{
    
}
+ (void) enableMicCapture
{
    [[MobCrush sharedInstance] enableMicCapture];
}
+ (void) disableMicCapture
{
    [[MobCrush sharedInstance] disableMicCapture];
}
+ (void) setMicrophoneGain: (float) gain
{
    [[MobCrush sharedInstance] setMicrophoneGain:gain];
}
+ (void) setGameGain: (float) gain
{
    [[MobCrush sharedInstance] setGameGain:gain];
}
// -----------------------------------------------------------------------------
// instance methods
// -----------------------------------------------------------------------------

- (id) init
{
    if((self = [super init]))
    {
        m_enableMicCapture = false;
        m_gameGain = 1.f;
        m_micGain = 1.f;
    }
    return self;
}
- (void) dealloc
{
    m_session.reset();
    [super dealloc];
}
- (BOOL) connectWithSettings: (NSDictionary*) settings error: (NSError**) error
{
    if(m_session) {
        [self disconnect];
    }
    if(![settings objectForKey:MCVideoBitrateKey] ||
       ![settings objectForKey:MCVideoFramerateKey] ||
       ![settings objectForKey:MCVideoHeightKey] ||
       ![settings objectForKey:MCVideoWidthKey] ) {
        if(error) {
            *error = [NSError errorWithDomain:@"MobCrush connect: Missing required video settings information" code:1000 userInfo:nil];
        }
        return false;
    }
    
    
    CGSize videoSize = CGSizeMake([[settings objectForKey:MCVideoWidthKey] floatValue],
                                  [[settings objectForKey:MCVideoHeightKey] floatValue]);
    
    m_videoSize = videoSize;
    
    if(![settings objectForKey:MCRTMPServerURIKey]) {
        if(error) {
            *error = [NSError errorWithDomain:@"MobCrush connect: Missing required server settings information" code:1000 userInfo:nil];
        }
        return false;
    }
    
    
    NSString *rtmpUrl = [[settings objectForKey:MCRTMPServerURIKey] stringByAppendingString:[NSString stringWithFormat:@"/%@",[settings objectForKey:MCRTMPServerStreamKey]]];
    
    std::string url([rtmpUrl UTF8String]);
    
    m_session.reset(new mobcrush::MobcrushSession([self](mobcrush::SessionState_t state){
        [self connectionStatusChange:state];
    }));
    
    m_session->startRtmpSession([rtmpUrl UTF8String], videoSize.width, videoSize.height, [[settings objectForKey:MCVideoBitrateKey] intValue], [[settings objectForKey:MCVideoFramerateKey] intValue]);
    
    m_session->microphoneEnabled(m_enableMicCapture);
    m_session->microphoneGain(m_micGain);
    m_session->gameGain(m_gameGain);

    
    return true;
}
- (void) connectionStatusChange: (mobcrush::SessionState_t) status {
    if(status == mobcrush::kSessionStateStarted) {
        if(m_enableCameraCapture) {
            m_session->cameraEnable(m_cameraRect.origin.x, m_cameraRect.origin.y, m_cameraRect.size.width, m_cameraRect.size.height, m_cameraAspect);
        }
    }
    else if ( status == mobcrush::kSessionStateEnded || status == mobcrush::kSessionStateError) {
        NSLog(@"Killing mobcrush session");
        m_session.reset();
    }
    [[NSNotificationCenter defaultCenter] postNotificationName:(NSString*)MCConnectionStateChangedNotification object:@(status)];

    if(s_callback) {
        s_callback(status);
    }
    
}
- (void) disconnect
{
    m_session.reset();
}
- (void) enableMicCapture
{
    m_enableMicCapture = true;
    if(m_session) {
        m_session->microphoneEnabled(true);
    }
}
- (void) disableMicCapture
{
    m_enableMicCapture = false;
    if(m_session) {
        m_session->microphoneEnabled(false);
    }
}
- (void) setMicrophoneGain: (float) gain
{
    m_micGain = gain;
    if(m_session) {
        m_session->microphoneGain(gain);
    }
}
- (void) setGameGain: (float) gain {
    m_gameGain = gain;
    if(m_session) {
        m_session->gameGain(gain);
    }
}
- (void) enableCameraCaptureWithRect: (CGRect) location {
    m_enableCameraCapture = true;
    
#define CLAMP(x) MAX(0.f,MIN(1.f,x))
    
    m_cameraRect = CGRectMake(CLAMP(location.origin.x/m_videoSize.width), CLAMP(location.origin.y/m_videoSize.height), CLAMP(location.size.width/m_videoSize.width), CLAMP(location.size.height/m_videoSize.height));
    
    m_cameraAspect = location.size.width / location.size.height;
    
    if(m_session) {
        m_session->cameraEnable(m_cameraRect.origin.x, m_cameraRect.origin.y, m_cameraRect.size.width, m_cameraRect.size.height, m_cameraAspect);
    }
}
- (void) disableCameraCapture {
    m_enableCameraCapture = false;
    if(m_session) {
        m_session->cameraDisable();
    }
}
@end