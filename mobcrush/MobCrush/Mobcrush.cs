using UnityEngine;
using System.Runtime.InteropServices;



class Mobcrush : MonoBehaviour {
	
	#if UNITY_IPHONE

	
	// Callback when the connection state changes. See MCConstants.h for status values.
	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    delegate void (*mobcrush_connection_callback_t)(int mobcrushConnectionState);

    
	/*
     * @brief Connect to an RTMP server and begin broadcasting.
     *
     * @param server_uri  The server URI and App path. i.e. "rtmp://192.168.1.1/myApp"
     * @param stream_key  The stream key / playpath.  i.e. "myStream"
     * @param bitrate     Desired video bitrate
     * @param fps         Desired video fps
     * @param width       Desired video width
     * @param height      Desired video height
     * @param callback    Callback to be called when the connection state changes. Can be NULL.
     *
     * @return 0 on failure, 1 on success
     */
    [DllImport("__Internal")]
    private static extern int  mobcrush_connect(const char* server_uri, const char* stream_key, int bitrate, int fps, int width, int height, mobcrush_connection_callback_t callback);
    

    /*!
     *  @brief Disconnect from the server.
     */
    [DllImport("__Internal")]
    private static extern void mobcrush_disconnect();
    
    /*!
     *  @brief Enabable microphone capture
     */
    [DllImport("__Internal")]
    private static extern void mobcrush_enable_mic();
    
    /*!
     *  @brief  Disable microphone capture
     */
    [DllImport("__Internal")]
    private static extern void mobcrush_disable_mic();
    
    /*! 
     *  @brief Enable capture from the device's front-facing camera.
     *
     *  The screen origin is the top-left corner. The rect origin is the center.
     *
     *  The aspect ratio of the video will be corrected depending on the camera's input values, but it will fit within
     *  the specified rectangle.  The origin will not change.
     *
     *  @param x    The x value for the origin within the video.
     *  @param y    The y value for the origin within the video.
     *  @param w    The width of the bounding box.
     *  @param h    The height of the bounding box.
     */
    [DllImport("__Internal")]
    private static extern void mobcrush_enable_camera(float x, float y, float w, float h);
    
    /*!
     *  @brief Disable capture from the device's front-facing camera.
     */
    [DllImport("__Internal")]
    private static extern void mobcrush_disable_camera();
    
    /*!
     *  @brief Adjust gain for microphone input.
     *
     *  Values will be clamped to between 0.0 and 1.0
     *
     *  @param gain  Desired gain for the microphone. Default is 1.0.
     *
     */
    [DllImport("__Internal")]
    private static extern void mobcrush_mic_gain(float gain);

    /*!
     *  @brief Adjust gain for game input.
     *
     *  Values will be clamped to between 0.0 and 1.0
     *
     *  @param gain  Desired gain for the game. Default is 1.0.
     *
     */
    [DllImport("__Internal")]
    private static extern void mobcrush_game_gain(float gain);

    #endif


}