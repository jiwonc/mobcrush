               _                   _   
     _____ ___| |_ ___ ___ _ _ ___| |_ 
    |     | . | . |  _|  _| | |_ -|   |
    '-'-'-'---'---'---'-' '---'---'-'-'
    
  
iOS SDK to facilitate game streaming to sites such as uStream, Twitch, LiveStream, etc.


Version 0.1a1
----

- First version, Proof of Concept.
- Will connect to RTMP service.
- Captures Audio Units output to speaker
- Captures OpenGL ES context
- Streams audio and video

Known Issues:
- Video orientation is completely dependent on OpenGL ES context orientation
- No mic capture [ to be implemented ]
- No camera capture [ to be implemented ]
- Audio and Video may not stay synchronized correctly
- 8-bit audio will not behave correctly
- No OpenAL or AVAudioPlayer compatibility [ to be implemented ] 
