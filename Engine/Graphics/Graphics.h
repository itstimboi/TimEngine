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
    // gl121,
    gl12,
    gl11,
    gl10
};

Renderer bestGLVersion();

bool isUsingGL1 = false;

bool madUnkVersionGL();