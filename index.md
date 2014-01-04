---
layout: index
---

Version 0.2a3
----

- You can now enable and disable microphone capture
- You can now independently control the game gain and microphone gain
- RTMP timestamps are now assured to be monotonic


Version 0.1a2
----

- Stability enhancements thanks to moving from prototype code to an architecture more suitable to build a product with.
- Audio and video will now remain synchronized
- Can now resample 8, 16, 24, and 32-bit audio.
- Can now resample stereo or mono audio.


Version 0.1a1
----

- First version, Proof of Concept.
- Will connect to RTMP service.
- Captures Audio Units output to speaker
- Captures OpenGL ES context
- Streams audio and video

Known Issues:
- Video orientation is completely dependent on OpenGL ES context orientation
- No mic capture  -> (*to be implemented*)
- No camera capture -> (*to be implemented*)
- Audio and Video may not stay synchronized correctly
- 8-bit audio will not behave correctly
- No OpenAL or AVAudioPlayer compatibility -> (*to be implemented*)
