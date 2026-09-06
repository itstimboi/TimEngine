// #include <filesystem>
// #include <string>
// #include <iostream>
// #include <list>
// #include <math.h>

// #include "ImGUI/imgui.h"
// #include "ImGUI/imgui_impl_glfw.h"
// #include "ImGUI/imgui_impl_opengl3.h"

// #define GLM_ENABLE_EXPERIMENTAL
// #include "Mesh/Model.h"

// #define STB_VORBIS_IMPLEMENTATION
// #include "Audio/Audio.h"

// #include "Map/Map.h"

// #include "Sprites/Sprite.h"

// uint16_t windowResolution[] = 
// {
//     1280,
//     720
// };

// std::string windowTitle = "Test";


// void framebuffer_size_callback(GLFWwindow* window, int width, int height)
// {
//     glViewport(0, 0, width, height);
// }

// void ImGuiShenanagins(bool isInit, GLFWwindow* window)
// {
//     if (isInit)
//     {
//         IMGUI_CHECKVERSION();
//         ImGui::CreateContext();
//         ImGuiIO& io = ImGui::GetIO(); (void)io;
//         ImGui::StyleColorsDark();
//         ImGui_ImplGlfw_InitForOpenGL(window, true);
//         ImGui_ImplOpenGL3_Init("#version 460");
//     }
//     else
//     {
//         glDisable(GL_CULL_FACE);
//         glDisable(GL_DEPTH_TEST);
//         ImGui_ImplOpenGL3_NewFrame();
//         ImGui_ImplGlfw_NewFrame();
//         ImGui::NewFrame();

//         ImGui::Begin("Window");
        
//         // // ImGui::ColorPicker3("Background Color", bgColor);
//         // std::string mapsPath = "Game/Maps/Tests/";

//         // std::vector<std::string> maps = listFiles(mapsPath);

//         // if (ImGui::Combo("Map File", &selectedMap, maps[selectedMap].c_str()))
//         // {
            
//         // }

//         ImGui::End();

//         ImGui::Render();
//         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//         glEnable(GL_DEPTH_TEST);
//         glEnable(GL_CULL_FACE);
//     }
// }

// // Vertices coordinates
// Vertex vertices[] =
// { //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
// 	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
// 	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
// 	Vertex{glm::vec3( 1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
// 	Vertex{glm::vec3( 1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
// };

// Indices for vertices order
// GLuint indices[] =
// {
// 	0, 1, 2,
// 	0, 2, 3
// };

// Vertex lightVertices[] =
// { //     COORDINATES     //
// 	Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
// 	Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
// 	Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
// 	Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
// 	Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
// 	Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
// 	Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
// 	Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
// };

// GLuint lightIndices[] =
// {
// 	0, 1, 2,
// 	0, 2, 3,
// 	0, 4, 7,
// 	0, 7, 3,
// 	3, 7, 6,
// 	3, 6, 2,
// 	2, 6, 5,
// 	2, 5, 1,
// 	1, 5, 4,
// 	1, 4, 0,
// 	4, 5, 6,
// 	4, 6, 7
// };

#include "Graphics/Graphics.h"

#include "messageBox.h"

int main()
{
    if (!InitGraphics(
        1280,
        720,
        "TimEngine"
    ))
    {
        CreateMessageBox("Couldn't Create Window, Shutting Down...", "ERROR", MESBOX_OK, MESBOX_WARNINGICON);
        return 1;
    }

    while (!GraphicsShouldClose())
    {
        PollGraphicsEvents();

        

        SwapGraphicsBuffers();
    }

    DeInitGraphics();

    return 0;
}