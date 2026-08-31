#pragma once

enum class Renderer
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

extern bool isUsingGL1;

Renderer bestGLVersion();

bool InitGraphics(int width, int height, const char* title);
void DeInitGraphics();

Renderer GetRenderer();

void SwapGraphicsBuffers();
bool GraphicsShouldClose();
void PollGraphicsEvents();