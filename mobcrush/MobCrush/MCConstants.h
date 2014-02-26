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
// [Required] Key to set the video bitrate. Takes an NSNumber value
extern const NSString* MCVideoBitrateKey;

// [Required] Key to set the video framerate i.e. 15, 24, 30, 60.  Takes an NSNumber value
extern const NSString* MCVideoFramerateKey;

// [Required] Key to set the video width key.  Takes an NSNumber value.
extern const NSString* MCVideoWidthKey;

// [Required] Key to set the video height key.  Takes an NSNumber value.
extern const NSString* MCVideoHeightKey;

// [Required] Key to set the target RTMP server URI and App path. eg. "rtmp://192.168.1.1/myApp"  Takes an NSString value.
extern const NSString* MCRTMPServerURIKey;

// [Required] Key to set the target RTMP server playPath / stream key. eg. "myStream"
extern const NSString* MCRTMPServerStreamKey;


// Notification that is dispatched when the connection state changes.  The object passed with the notification will be one of the
// values specified below encapsulated in an NSNumber.
extern const NSString* MCConnectionStateChangedNotification;

// Connection state values.  These are encapsulated in an NSNumber when passed with the MCConectionStateChangedNotification.
enum {
    kMobcrushConnectionStateNotConnected=0,
    kMobcrushConnectionStateStarted,
    kMobcrushConnectionStateEnded,
    kMobcrushConnectionStateError
};