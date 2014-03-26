           _                   _   
 _____ ___| |_ ___ ___ _ _ ___| |_ 
|     | . | . |  _|  _| | |_ -|   |
'-'-'-'---'---'---'-' '---'---'-'-'
          (c) 2013-2014  James G Hurley

iOS SDK to facilitate game streaming to sites such as uStream, Twitch, Hitbox.tv, and other RTMP-based services.

Setup
=====

git clone git@github.com:jamesghurley/mobcrush.git
cd mobcrush
git submodule init
git submodule update --init --recursive


Once you build the Mobcrush project and import libmobcrush.a and Mobcrush.h into your project, you will need to link
the following frameworks:

- AVFoundation.framework
- AudioToolbox.framework
- CoreGraphics.framework
- QuartzCore.framework
- CoreVideo.framework

Additionally if your project isn't using C++11, you'll need to add the following linker flag to the build settings:
-lc++

=====

  ________________________________________________________________
 |   __________________    ______________   ___________________   |          ______________
 |  |                  |  |              | |                   |  |         |              |
 |  | Camera Capture   |  | Mic Capture  | | OpenGL CTX/Capture| <----------| Client GLES  |
 |  |                  |  |              | |                   |  |         |______________|
 |  |__________________|  |______________| |___________________|  |
 |         |     ,------------|---------------'                   |
 |    _____v_____v______  ____v___________________                |          ______________
 |   |                  ||                        |               |         |              |
 |   |   Video Mix/Enc  || Audio Mix/Resample/Enc | <-----------------------| Client Audio |
 |   |__________________||________________________|               |         |______________|
 |            |               |                                   |
 |    ________v_______________v_________                          |
 |   |                                  |       mobcrush          |
 |   |      RTMP Multiplex/Transmit     |        SDK              |
 |   |__________________________________|                         |
 |_________________|______________________________________________|
                   |
                   v
      __________________________________
     |                                  |
     |          RTMP Server             |
     |__________________________________|



LICENSING
=========

Mobcrush and the associated VideoCore library are licensed under LGPL 2.1.  I would like to make
it clear that I explicitly give my permission for users of this library to statically link
Mobcrush and VideoCore for the purposes of using them with iOS Apps, as long as the developer provides
an object file for download that can be relinked against a modified version of Mobcrush.


Version 0.3a6
----
*Jan. 9, 2014*

- RGB565 Renderbuffers now work correctly.
- Improved GLES Render Time.
- This is the last alpha release.  To sign up for Beta, please e-mail james@mobcrush.com. 

Version 0.3a5
----
*Jan. 8, 2014*

- You can now capture video from the front-facing camera and it will be composited into the outgoing RTMP stream.
- API Cleanup / Documentation

Version 0.2a4
----
*Jan. 6, 2014*

- Fixes a bug where the H.264 encoder instance would occasionally not start
- Cleanup API a bit
- Adds a new C API for projects not using Objective C.


Version 0.2a3
----
*Jan. 3, 2014*

- You can now enable and disable microphone capture
- You can now independently control the game gain and microphone gain
- RTMP timestamps are now assured to be monotonic


Version 0.1a2
----
*Dec. 30, 2013*

- Stability enhancements thanks to moving from prototype code to an architecture more suitable to build a product with.
- Audio and video will now remain synchronized
- Can now resample 8, 16, 24, and 32-bit audio.
- Can now resample stereo or mono audio.


Version 0.1a1
----
*Nov. 24, 2013*

- First version, Proof of Concept.
- Will connect to RTMP service.
- Captures Audio Units output to speaker
- Captures OpenGL ES context
- Streams audio and video

Target 1.0 Features
----

- OpenGL ES Capture ✓
- CoreAudio Capture ✓
- OpenAL Capture
- Microphone Capture ✓
- Audio Mixing/Resampling ✓
- Camera Capture ✓
- Game logo/watermark
- Video layer compositing ✓
- H.264 Hardware-based Encode ✓
- AAC Hardware-based Encode ✓
- RTMP Publish/Broadcast ✓
