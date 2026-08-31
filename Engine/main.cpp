#include <filesystem>
#include <string>
#include <iostream>
#include <list>
#include <math.h>

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "Mesh/Model.h"

#define STB_VORBIS_IMPLEMENTATION
#include "Audio/Audio.h"

#include "Map/Map.h"

#include "Sprites/Sprite.h"

uint16_t windowResolution[] = 
{
    1280,
    720
};

std::string windowTitle = "Test";

bool isOpenGLVersionSupported(int major, int minor) {
    // 1. Create a temporary context requesting the target version
    // Example using GLFW (replace with WGL/GLX if not using GLFW)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    
    GLFWwindow* tempWindow = glfwCreateWindow(100, 100, "Temp", NULL, NULL);
    
    if (!tempWindow) return false; // Context creation failed

    glfwMakeContextCurrent(tempWindow);

    // 2. Query the actual version granted by the driver
    const GLubyte* versionString = glGetString(GL_VERSION);
    if (!versionString) return false;

    int grantedMajor, grantedMinor;
    // Parse the version string (e.g., "3.3.0 NVIDIA...")
    if (sscanf((const char*)versionString, "%d.%d", &grantedMajor, &grantedMinor) != 2) {
        glfwDestroyWindow(tempWindow);
        return false;
    }

    // 3. Check if the granted version meets the requested minimum
    bool supported = (grantedMajor > major) || 
                     (grantedMajor == major && grantedMinor >= minor);

    glfwDestroyWindow(tempWindow);
    return supported;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void ImGuiShenanagins(bool isInit, GLFWwindow* window)
{
    if (isInit)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");
    }
    else
    {
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Window");
        
        // // ImGui::ColorPicker3("Background Color", bgColor);
        // std::string mapsPath = "Game/Maps/Tests/";

        // std::vector<std::string> maps = listFiles(mapsPath);

        // if (ImGui::Combo("Map File", &selectedMap, maps[selectedMap].c_str()))
        // {
            
        // }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }
}

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

GLuint samples = 8;

int main()
{

    if (!glfwInit())
    {
        std::cout << "Failed to start GLFW." << std::endl;
        return -1;
    }
	
	if (isOpenGLVersionSupported(4, 6))
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	}
    glfwWindowHint(GLFW_SAMPLES, samples);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(windowResolution[0], windowResolution[1], windowTitle.c_str(), nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if(!gladLoadGL())
    {
        std::cout << "Failed to start glad." << std::endl;
        return -1;
    }

    glViewport(0, 0, windowResolution[0], windowResolution[1]);

    if (!Audio::Init())
    {
        return -1;
    }

    if (!Map::Init())
    {
        return -1;
    }

    Shader shaderProgram("Shaders/defaultvert.glsl", "Shaders/defaultfrag.glsl");
    // Shader shaderProgram("Shaders/default.vert", "Shaders/default.frag");
    // Shader instancingShdader("Shaders/instancing.vert", "Shaders/default.frag");

    ImGuiShenanagins(true, window);


    Shader lightShader("Shaders/lightvert.glsl", "Shaders/lightfrag.glsl");

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f, 2.0f, -5.0f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    Camera camera(windowResolution[0], windowResolution[1], glm::vec3(0.0f, 0.0f, 0.0f));

    lightShader.SetInt("usingLights", true);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    SoundHandle testFNAF2 =
    Audio::Pre_CacheSound("Game/Sounds/Tests/FNAF2.wav");
    
    Audio::PlaySound(testFNAF2, true);

    MapHandle testMap = 
    Map::Pre_CacheMap("Game/Maps/Tests/testMap.tmap");

    // GLfloat bgColor[] = 
    // {
    //     1.0f,
    //     1.0f,
    //     1.0f
    // };

    Sprite newSprite("Editor/Sprites/light.png");

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        camera.Inputs(window);

        camera.updateMatrix(45.0f, 0.01f, 1000.0f);

        Map::Draw(testMap, shaderProgram, camera);
        
        newSprite.Draw(glm::vec3(1.0f), glm::vec3(1.0f), 0.0f, shaderProgram, camera);

        ImGuiShenanagins(false, nullptr);

        glfwSwapBuffers(window);
    }

    shaderProgram.Delete();
	lightShader.Delete();

    Audio::Shutdown();
    Map::Shutdown();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}