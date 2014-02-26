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

#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>
#import <MobCrush/MCConstants.h>

@class EAGLContext;

@interface MobCrush : NSObject

/*!
 *  Connect to the service specified in the settings dictionary.  See MCConstants.h for keys.
 *
 *  @param settings     A dictionary containing server and video settings.
 *  @param error        An object to return an error to.  nil if no error.
 *
 *  @return A boolean indicating if there was an error encountered.
 */
+ (BOOL) connectWithSettings: (NSDictionary*) settings error: (NSError**) error;

/*!
 *  Disconnect from the service.
 */
+ (void) disconnect;

/*!
 *  Exclude an EAGL context from being captured by Mobcrush.
 *
 *  @param context  The context to exclude
 */
+ (void) excludeEAGLContextFromCapture: (EAGLContext*) context;

/*!
 *
 *  @brief Start capturing from the device's front-facing camera.  The captured output will appear in the video stream.
 *
 *  location represents the position in the video stream, so location will be clamped to the video size you specified in the settings.
 *
 *  The screen origin is the top-left corner. The rect origin is the center.
 *
 *  The aspect ratio of the video will be corrected depending on the camera's input values, but it will fit within
 *  the specified rectangle.  The origin will not change.
 *
 *
 *  @param location  The rectangle that the captured video should fit within.  Based on the values specified in connectWithSettings.
 */
+ (void) enableCameraCaptureWithRect: (CGRect) location;

/*
 * @brief Stop capturing with the camera.
 *
 */
+ (void) disableCameraCapture;

/*!
 *  @brief Enable microphone capture
 */
+ (void) enableMicCapture;

/*!
 *  @brief Disable microphone capture
 */
+ (void) disableMicCapture;

/*!
 *  @brief Adjust gain for microphone input.
 *
 *  Values will be clamped to between 0.0 and 1.0
 *
 *  @param gain  Desired gain for the microphone. Default is 1.0.
 *
 */
+ (void) setMicrophoneGain: (float) gain;

/*!
 *  @brief Adjust gain for game input.
 *
 *  Values will be clamped to between 0.0 and 1.0
 *
 *  @param gain  Desired gain for the game. Default is 1.0.
 *
 */
+ (void) setGameGain: (float) gain;

@end
