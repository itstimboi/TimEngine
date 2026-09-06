// #pragma once

// enum class Renderer
// {
//     gl,

//     dx,

//     vk,

//     software,

//     None
// };

// enum class GLRenderer
// {
//     gl46,
//     gl45,
//     gl44,
//     gl43,
//     gl42,
//     gl41,
//     gl40,

//     gl33,
//     gl32,
//     gl31,
//     gl30,

//     gl21,
//     gl20,

//     gl15,
//     gl14,
//     gl13,
//     gl12,
//     gl11,
//     gl10,

//     None
// };

// enum class DXRenderer
// {
//     dx120,
//     dx110,
//     dx100,

//     dx90c,
//     dx90b,
//     dx90a,
//     dx90,

//     dx81b,
//     dx81,

//     dx70a,
//     dx70,

//     dx61,
//     dx60,

//     dx52a,
//     dx52,

//     dx51,
//     dx50,
    
//     dx30a,
//     dx30,
    
//     dx20,
    
//     dx10,

//     None
// };

// enum class VKRenderer
// {
//     vk14,

//     vk13,

//     None
// };

// extern bool isUsingGL1;

// Renderer bestGLVersion();

// bool InitGraphics(int width, int height, const char* title);
// void DeInitGraphics();

// Renderer GetRenderer();

// void SwapGraphicsBuffers();
// bool GraphicsShouldClose();
// void PollGraphicsEvents();

#pragma once

enum class Renderer
{
    GL,
    DX,
    VK,
    Software,
    None
};

enum class GLRenderer
{
    gl46,
    gl45,
    gl44,
    gl43,
    gl42,
    gl41,
    gl40,

    gl33,
    gl32,
    gl31,
    gl30,

    gl21,
    gl20,

    gl15,
    gl14,
    gl13,
    gl12,
    gl11,
    gl10,

    None
};

enum class DXRenderer
{
    dx120,

    dx110,

    dx100,

    dx90c,
    dx90b,
    dx90a,
    dx90,

    dx81b,
    dx81,
    dx80,

    dx70a,
    dx70,

    dx61,
    dx60,

    dx52a,
    dx52,
    dx51,
    dx50,

    dx30a,
    dx30,

    dx20,

    dx10,

    None
};

enum class VKRenderer
{
    vk14,
    vk13,

    None
};


// True when the selected OpenGL renderer is using
// the old OpenGL 1.x-style API.
bool IsUsingLegacyOpenGL();

static bool isUsingGL1 = false;

// Detection
GLRenderer bestGLVersion();


// Graphics
bool InitGraphics(
    int width,
    int height,
    const char* title
);

void DeInitGraphics();


// Information
Renderer GetRenderer();

GLRenderer GetGLRenderer();
DXRenderer GetDXRenderer();
VKRenderer GetVKRenderer();


// Window
void SwapGraphicsBuffers();
bool GraphicsShouldClose();
void PollGraphicsEvents();