
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
