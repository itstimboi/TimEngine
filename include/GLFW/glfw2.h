#ifndef __glfw2_h_
#define __glfw2_h_

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------- */
/* Platform definitions                                                      */
/* ------------------------------------------------------------------------- */

#if defined(_WIN32) && !defined(APIENTRY)
    #define APIENTRY __stdcall
#endif

#if defined(_WIN32) && !defined(WINGDIAPI)
    #define WINGDIAPI __declspec(dllimport)
#endif

#if defined(_WIN32) && !defined(CALLBACK)
    #define CALLBACK __stdcall
#endif

/* ------------------------------------------------------------------------- */
/* GLFW2 API declarations                                                    */
/* ------------------------------------------------------------------------- */

#if defined(_WIN32)
    #define GLFW2APIENTRY APIENTRY
    #define GLFW2CALL     __cdecl

    #ifdef GLFW2_BUILD_DLL
        #define GLFW2API __declspec(dllexport)
    #else
        #define GLFW2API __declspec(dllimport)
    #endif
#else
    #define GLFW2APIENTRY
    #define GLFW2CALL
    #define GLFW2API
#endif

/* ------------------------------------------------------------------------- */
/* OpenGL                                                                     */
/* ------------------------------------------------------------------------- */

#if defined(__APPLE__)
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

/* ------------------------------------------------------------------------- */
/* GLFW2 version                                                              */
/* ------------------------------------------------------------------------- */

#define GLFW2_VERSION_MAJOR 2
#define GLFW2_VERSION_MINOR 5
#define GLFW2_VERSION_REVISION 0

/* ------------------------------------------------------------------------- */
/* Keyboard                                                                    */
/* ------------------------------------------------------------------------- */

#define GLFW2_KEY_UNKNOWN      -1

#define GLFW2_KEY_SPACE        32
#define GLFW2_KEY_SPECIAL      256

#define GLFW2_KEY_ESC          (GLFW2_KEY_SPECIAL + 1)
#define GLFW2_KEY_F1           (GLFW2_KEY_SPECIAL + 2)
#define GLFW2_KEY_F2           (GLFW2_KEY_SPECIAL + 3)
#define GLFW2_KEY_F3           (GLFW2_KEY_SPECIAL + 4)
#define GLFW2_KEY_F4           (GLFW2_KEY_SPECIAL + 5)
#define GLFW2_KEY_F5           (GLFW2_KEY_SPECIAL + 6)
#define GLFW2_KEY_F6           (GLFW2_KEY_SPECIAL + 7)
#define GLFW2_KEY_F7           (GLFW2_KEY_SPECIAL + 8)
#define GLFW2_KEY_F8           (GLFW2_KEY_SPECIAL + 9)
#define GLFW2_KEY_F9           (GLFW2_KEY_SPECIAL + 10)
#define GLFW2_KEY_F10          (GLFW2_KEY_SPECIAL + 11)
#define GLFW2_KEY_F11          (GLFW2_KEY_SPECIAL + 12)
#define GLFW2_KEY_F12          (GLFW2_KEY_SPECIAL + 13)

#define GLFW2_KEY_UP           (GLFW2_KEY_SPECIAL + 14)
#define GLFW2_KEY_DOWN         (GLFW2_KEY_SPECIAL + 15)
#define GLFW2_KEY_LEFT         (GLFW2_KEY_SPECIAL + 16)
#define GLFW2_KEY_RIGHT        (GLFW2_KEY_SPECIAL + 17)

#define GLFW2_KEY_LSHIFT       (GLFW2_KEY_SPECIAL + 18)
#define GLFW2_KEY_RSHIFT       (GLFW2_KEY_SPECIAL + 19)
#define GLFW2_KEY_LCTRL        (GLFW2_KEY_SPECIAL + 20)
#define GLFW2_KEY_RCTRL        (GLFW2_KEY_SPECIAL + 21)
#define GLFW2_KEY_LALT         (GLFW2_KEY_SPECIAL + 22)
#define GLFW2_KEY_RALT         (GLFW2_KEY_SPECIAL + 23)

#define GLFW2_KEY_TAB          (GLFW2_KEY_SPECIAL + 24)
#define GLFW2_KEY_ENTER        (GLFW2_KEY_SPECIAL + 25)
#define GLFW2_KEY_BACKSPACE    (GLFW2_KEY_SPECIAL + 26)
#define GLFW2_KEY_INSERT       (GLFW2_KEY_SPECIAL + 27)
#define GLFW2_KEY_DEL          (GLFW2_KEY_SPECIAL + 28)
#define GLFW2_KEY_PAGEUP       (GLFW2_KEY_SPECIAL + 29)
#define GLFW2_KEY_PAGEDOWN     (GLFW2_KEY_SPECIAL + 30)
#define GLFW2_KEY_HOME         (GLFW2_KEY_SPECIAL + 31)
#define GLFW2_KEY_END          (GLFW2_KEY_SPECIAL + 32)

#define GLFW2_KEY_KP_0         (GLFW2_KEY_SPECIAL + 33)
#define GLFW2_KEY_KP_1         (GLFW2_KEY_SPECIAL + 34)
#define GLFW2_KEY_KP_2         (GLFW2_KEY_SPECIAL + 35)
#define GLFW2_KEY_KP_3         (GLFW2_KEY_SPECIAL + 36)
#define GLFW2_KEY_KP_4         (GLFW2_KEY_SPECIAL + 37)
#define GLFW2_KEY_KP_5         (GLFW2_KEY_SPECIAL + 38)
#define GLFW2_KEY_KP_6         (GLFW2_KEY_SPECIAL + 39)
#define GLFW2_KEY_KP_7         (GLFW2_KEY_SPECIAL + 40)
#define GLFW2_KEY_KP_8         (GLFW2_KEY_SPECIAL + 41)
#define GLFW2_KEY_KP_9         (GLFW2_KEY_SPECIAL + 42)
#define GLFW2_KEY_KP_DIVIDE    (GLFW2_KEY_SPECIAL + 43)
#define GLFW2_KEY_KP_MULTIPLY  (GLFW2_KEY_SPECIAL + 44)
#define GLFW2_KEY_KP_SUBTRACT  (GLFW2_KEY_SPECIAL + 45)
#define GLFW2_KEY_KP_ADD       (GLFW2_KEY_SPECIAL + 46)
#define GLFW2_KEY_KP_DECIMAL   (GLFW2_KEY_SPECIAL + 47)
#define GLFW2_KEY_KP_EQUAL     (GLFW2_KEY_SPECIAL + 48)
#define GLFW2_KEY_KP_ENTER     (GLFW2_KEY_SPECIAL + 49)

#define GLFW2_KEY_KP_NUM_LOCK  (GLFW2_KEY_SPECIAL + 50)
#define GLFW2_KEY_CAPS_LOCK    (GLFW2_KEY_SPECIAL + 51)
#define GLFW2_KEY_SCROLL_LOCK  (GLFW2_KEY_SPECIAL + 52)
#define GLFW2_KEY_PAUSE        (GLFW2_KEY_SPECIAL + 53)
#define GLFW2_KEY_LSUPER       (GLFW2_KEY_SPECIAL + 54)
#define GLFW2_KEY_RSUPER       (GLFW2_KEY_SPECIAL + 55)
#define GLFW2_KEY_MENU         (GLFW2_KEY_SPECIAL + 56)

#define GLFW2_KEY_LAST         GLFW2_KEY_MENU

/* ------------------------------------------------------------------------- */
/* Mouse                                                                       */
/* ------------------------------------------------------------------------- */

#define GLFW2_MOUSE_BUTTON_1   0
#define GLFW2_MOUSE_BUTTON_2   1
#define GLFW2_MOUSE_BUTTON_3   2
#define GLFW2_MOUSE_BUTTON_4   3
#define GLFW2_MOUSE_BUTTON_5   4
#define GLFW2_MOUSE_BUTTON_6   5
#define GLFW2_MOUSE_BUTTON_7   6
#define GLFW2_MOUSE_BUTTON_8   7

#define GLFW2_MOUSE_BUTTON_LEFT   GLFW2_MOUSE_BUTTON_1
#define GLFW2_MOUSE_BUTTON_RIGHT  GLFW2_MOUSE_BUTTON_2
#define GLFW2_MOUSE_BUTTON_MIDDLE GLFW2_MOUSE_BUTTON_3

/* ------------------------------------------------------------------------- */
/* Joystick                                                                    */
/* ------------------------------------------------------------------------- */

#define GLFW2_JOYSTICK_1       0
#define GLFW2_JOYSTICK_2       1
#define GLFW2_JOYSTICK_3       2
#define GLFW2_JOYSTICK_4       3
#define GLFW2_JOYSTICK_5       4
#define GLFW2_JOYSTICK_6       5
#define GLFW2_JOYSTICK_7       6
#define GLFW2_JOYSTICK_8       7
#define GLFW2_JOYSTICK_9       8
#define GLFW2_JOYSTICK_10      9
#define GLFW2_JOYSTICK_11      10
#define GLFW2_JOYSTICK_12      11
#define GLFW2_JOYSTICK_13      12
#define GLFW2_JOYSTICK_14      13
#define GLFW2_JOYSTICK_15      14
#define GLFW2_JOYSTICK_16      15

#define GLFW2_JOYSTICK_LAST    GLFW2_JOYSTICK_16

/* ------------------------------------------------------------------------- */
/* Window modes                                                                */
/* ------------------------------------------------------------------------- */

#define GLFW2_WINDOW           0x00010001
#define GLFW2_FULLSCREEN       0x00010002

/* ------------------------------------------------------------------------- */
/* Window parameters                                                           */
/* ------------------------------------------------------------------------- */

#define GLFW2_OPENED           0x00020001
#define GLFW2_ACTIVE           0x00020002
#define GLFW2_ICONIFIED       0x00020003
#define GLFW2_ACCELERATED     0x00020004
#define GLFW2_RED_BITS        0x00020005
#define GLFW2_GREEN_BITS      0x00020006
#define GLFW2_BLUE_BITS       0x00020007
#define GLFW2_ALPHA_BITS      0x00020008
#define GLFW2_DEPTH_BITS      0x00020009
#define GLFW2_STENCIL_BITS    0x0002000A
#define GLFW2_REFRESH_RATE    0x0002000B
#define GLFW2_ACCUM_RED_BITS  0x0002000C
#define GLFW2_ACCUM_GREEN_BITS 0x0002000D
#define GLFW2_ACCUM_BLUE_BITS 0x0002000E
#define GLFW2_ACCUM_ALPHA_BITS 0x0002000F
#define GLFW2_AUX_BUFFERS     0x00020010
#define GLFW2_STEREO          0x00020011

/* ------------------------------------------------------------------------- */
/* Enable / disable                                                            */
/* ------------------------------------------------------------------------- */

#define GLFW2_MOUSE_CURSOR            0x00030001
#define GLFW2_STICKY_KEYS             0x00030002
#define GLFW2_STICKY_MOUSE_BUTTONS    0x00030003
#define GLFW2_SYSTEM_KEYS             0x00030004
#define GLFW2_KEY_REPEAT              0x00030005
#define GLFW2_AUTO_POLL_EVENTS       0x00030006

/* ------------------------------------------------------------------------- */
/* Threading                                                                    */
/* ------------------------------------------------------------------------- */

#define GLFW2_WAIT           0x00040001
#define GLFW2_NOWAIT         0x00040002

/* ------------------------------------------------------------------------- */
/* Joystick parameters                                                         */
/* ------------------------------------------------------------------------- */

#define GLFW2_PRESENT        0x00050001
#define GLFW2_AXES           0x00050002
#define GLFW2_BUTTONS        0x00050003

/* ------------------------------------------------------------------------- */
/* Image flags                                                                 */
/* ------------------------------------------------------------------------- */

#define GLFW2_BUILD_MIPMAPS  0x00060001
#define GLFW2_ALPHA4         0x00060002
#define GLFW2_ALPHA8         0x00060003
#define GLFW2_ALPHA12        0x00060004
#define GLFW2_ALPHA16        0x00060005
#define GLFW2_RGB4           0x00060006
#define GLFW2_RGB5           0x00060007
#define GLFW2_RGB8           0x00060008
#define GLFW2_RGB10          0x00060009
#define GLFW2_RGB12          0x0006000A
#define GLFW2_RGB16          0x0006000B
#define GLFW2_RGBA4          0x0006000C
#define GLFW2_RGBA5          0x0006000D
#define GLFW2_RGBA8          0x0006000E
#define GLFW2_RGBA10         0x0006000F
#define GLFW2_RGBA12         0x00060010
#define GLFW2_RGBA16         0x00060011

#define GLFW2_INFINITY        100000.0

/* ------------------------------------------------------------------------- */
/* Types                                                                       */
/* ------------------------------------------------------------------------- */

typedef struct
{
    int Width;
    int Height;
    int RedBits;
    int GreenBits;
    int BlueBits;
} GLFW2vidmode;


typedef struct
{
    int Width;
    int Height;
    int BytesPerPixel;
    void* Data;
} GLFW2image;


typedef void* GLFW2thread;
typedef void* GLFW2mutex;
typedef void* GLFW2cond;

/* ------------------------------------------------------------------------- */
/* Callback functions                                                          */
/* ------------------------------------------------------------------------- */

typedef void (GLFW2CALL * GLFW2windowsizefun)(int Width, int Height);
typedef int  (GLFW2CALL * GLFW2windowclosefun)(void);
typedef void (GLFW2CALL * GLFW2windowrefreshfun)(void);

typedef void (GLFW2CALL * GLFW2mousebuttonfun)(int Button, int Action);
typedef void (GLFW2CALL * GLFW2mouseposfun)(int X, int Y);
typedef void (GLFW2CALL * GLFW2mousewheelfun)(int Wheel);

typedef void (GLFW2CALL * GLFW2keyfun)(int Key, int Action);
typedef void (GLFW2CALL * GLFW2charfun)(int Character, int Action);

typedef void (GLFW2CALL * GLFW2threadfun)(void* Arg);

/* ------------------------------------------------------------------------- */
/* Initialization                                                              */
/* ------------------------------------------------------------------------- */

GLFW2API int GLFW2CALL glfw2Init(void);
GLFW2API void GLFW2CALL glfw2Terminate(void);
GLFW2API void GLFW2CALL glfw2GetVersion(
    int* Major,
    int* Minor,
    int* Rev
);

/* ------------------------------------------------------------------------- */
/* Window                                                                      */
/* ------------------------------------------------------------------------- */

GLFW2API int GLFW2CALL glfw2OpenWindow(
    int Width,
    int Height,
    int RedBits,
    int GreenBits,
    int BlueBits,
    int AlphaBits,
    int DepthBits,
    int StencilBits,
    int Mode
);

GLFW2API void GLFW2CALL glfw2OpenWindowHint(
    int Target,
    int Hint
);

GLFW2API void GLFW2CALL glfw2CloseWindow(void);

GLFW2API void GLFW2CALL glfw2SetWindowTitle(
    const char* Title
);

GLFW2API void GLFW2CALL glfw2GetWindowSize(
    int* Width,
    int* Height
);

GLFW2API void GLFW2CALL glfw2SetWindowSize(
    int Width,
    int Height
);

GLFW2API void GLFW2CALL glfw2SetWindowPos(
    int X,
    int Y
);

GLFW2API void GLFW2CALL glfw2IconifyWindow(void);
GLFW2API void GLFW2CALL glfw2RestoreWindow(void);

GLFW2API void GLFW2CALL glfw2SwapBuffers(void);

GLFW2API void GLFW2CALL glfw2SwapInterval(
    int Interval
);

GLFW2API int GLFW2CALL glfw2GetWindowParam(
    int Param
);

/* ------------------------------------------------------------------------- */
/* Window callbacks                                                            */
/* ------------------------------------------------------------------------- */

GLFW2API void GLFW2CALL glfw2SetWindowSizeCallback(
    GLFW2windowsizefun Callback
);

GLFW2API void GLFW2CALL glfw2SetWindowCloseCallback(
    GLFW2windowclosefun Callback
);

GLFW2API void GLFW2CALL glfw2SetWindowRefreshCallback(
    GLFW2windowrefreshfun Callback
);

/* ------------------------------------------------------------------------- */
/* Mouse callbacks                                                             */
/* ------------------------------------------------------------------------- */

GLFW2API void GLFW2CALL glfw2SetMouseButtonCallback(
    GLFW2mousebuttonfun Callback
);

GLFW2API void GLFW2CALL glfw2SetMousePosCallback(
    GLFW2mouseposfun Callback
);

GLFW2API void GLFW2CALL glfw2SetMouseWheelCallback(
    GLFW2mousewheelfun Callback
);

/* ------------------------------------------------------------------------- */
/* Keyboard callbacks                                                          */
/* ------------------------------------------------------------------------- */

GLFW2API void GLFW2CALL glfw2SetKeyCallback(
    GLFW2keyfun Callback
);

GLFW2API void GLFW2CALL glfw2SetCharCallback(
    GLFW2charfun Callback
);

/* ------------------------------------------------------------------------- */
/* Video modes                                                                 */
/* ------------------------------------------------------------------------- */

GLFW2API int GLFW2CALL glfw2GetVideoModes(
    GLFW2vidmode* List,
    int Maxcount
);

GLFW2API GLFW2vidmode GLFW2CALL glfw2GetDesktopMode(void);

/* ------------------------------------------------------------------------- */
/* Input                                                                       */
/* ------------------------------------------------------------------------- */

GLFW2API int GLFW2CALL glfw2GetKey(
    int Key
);

GLFW2API int GLFW2CALL glfw2GetMouseButton(
    int Button
);

GLFW2API void GLFW2CALL glfw2GetMousePos(
    int* X,
    int* Y
);

GLFW2API void GLFW2CALL glfw2SetMousePos(
    int X,
    int Y
);

GLFW2API int GLFW2CALL glfw2GetMouseWheel(void);

GLFW2API void GLFW2CALL glfw2SetMouseWheel(
    int Wheel
);

/* ------------------------------------------------------------------------- */
/* Joystick                                                                    */
/* ------------------------------------------------------------------------- */

GLFW2API int GLFW2CALL glfw2GetJoystickParam(
    int Joy,
    int Param
);

GLFW2API int GLFW2CALL glfw2GetJoystickPos(
    int Joy,
    float* Pos,
    int Numaxes
);

GLFW2API int GLFW2CALL glfw2GetJoystickButtons(
    int Joy,
    unsigned char* Buttons,
    int Numbuttons
);

/* ------------------------------------------------------------------------- */
/* Time                                                                        */
/* ------------------------------------------------------------------------- */

GLFW2API double GLFW2CALL glfw2GetTime(void);

GLFW2API void GLFW2CALL glfw2SetTime(
    double Time
);

/* ------------------------------------------------------------------------- */
/* Extension support                                                           */
/* ------------------------------------------------------------------------- */

GLFW2API int GLFW2CALL glfw2ExtensionSupported(
    const char* Extension
);

GLFW2API void* GLFW2CALL glfw2GetProcAddress(
    const char* Procname
);

/* ------------------------------------------------------------------------- */
/* Threading                                                                    */
/* ------------------------------------------------------------------------- */

GLFW2API GLFW2thread GLFW2CALL glfw2CreateThread(
    GLFW2threadfun Function,
    void* Arg
);

GLFW2API void GLFW2CALL glfw2DestroyThread(
    GLFW2thread Thread
);

GLFW2API GLFW2thread GLFW2CALL glfw2GetThreadID(void);

GLFW2API void GLFW2CALL glfw2WaitThread(
    GLFW2thread Thread,
    int Waitmode
);

GLFW2API GLFW2mutex GLFW2CALL glfw2CreateMutex(void);
GLFW2API void GLFW2CALL glfw2DestroyMutex(GLFW2mutex Mutex);

GLFW2API void GLFW2CALL glfw2LockMutex(
    GLFW2mutex Mutex
);

GLFW2API void GLFW2CALL glfw2UnlockMutex(
    GLFW2mutex Mutex
);

GLFW2API GLFW2cond GLFW2CALL glfw2CreateCond(void);
GLFW2API void GLFW2CALL glfw2DestroyCond(GLFW2cond Cond);

GLFW2API void GLFW2CALL glfw2WaitCond(
    GLFW2cond Cond,
    GLFW2mutex Mutex,
    double Timeout
);

GLFW2API void GLFW2CALL glfw2SignalCond(
    GLFW2cond Cond
);

GLFW2API void GLFW2CALL glfw2BroadcastCond(
    GLFW2cond Cond
);

/* ------------------------------------------------------------------------- */
/* Enable / disable                                                            */
/* ------------------------------------------------------------------------- */

GLFW2API void GLFW2CALL glfw2Enable(
    int Token
);

GLFW2API void GLFW2CALL glfw2Disable(
    int Token
);

/* ------------------------------------------------------------------------- */
/* Image / texture I/O                                                         */
/* ------------------------------------------------------------------------- */

GLFW2API int GLFW2CALL glfw2ReadImage(
    const char* Name,
    GLFW2image* Image,
    int Flags
);

GLFW2API void GLFW2CALL glfw2FreeImage(
    GLFW2image* Image
);

GLFW2API int GLFW2CALL glfw2LoadTexture2D(
    const char* Name,
    int Flags
);

/* ------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* __glfw2_h_ */