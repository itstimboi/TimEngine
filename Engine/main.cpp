#include "Graphics/Graphics.h"

int main()
{
    TE::GraphicsWindow* newWindow = TE::CreateGraphicsWindow(1280, 720, "TimEngine");

    while (!TE::GraphicsWindowShouldClose(newWindow))
    {
        TE::GraphicsPollEvents();

        TE::GraphicsSwapBuffers(newWindow);
    }


    TE::DeInitGraphics();

    TE::DestroyGraphicsWindow(newWindow);

    return 0;
}