#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>

#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER

#include <windows.h>

#include "raylib/raylib.h"
#include "ImGUI/imgui.h"
#include "rlImGUI/rlImGui.h"

std::string currentVersion = "App version: 0.0.6";

void PrintHelp( void )
{
    std::cout << "Available commands:\n";
    std::cout << "  help\n";
    std::cout << "  -help\n";
    std::cout << "  --help\n";
    std::cout << "  --version\n";
    std::cout << "  --buildEngine\n";
    std::cout << "  --buildLevelEditor\n";
    std::cout << "  --runEngine\n";
    std::cout << "  --runLevelEditor\n";
}

void deInit()
{
    rlImGuiShutdown();

    CloseWindow();
}

std::string consoleOutput;
std::mutex consoleMutex;

void RunCommand(const std::string& command)
{
    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = nullptr;

    HANDLE hRead = nullptr;
    HANDLE hWrite = nullptr;

    if (!CreatePipe(&hRead, &hWrite, &sa, 0))
        return;

    SetHandleInformation(hRead, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOA si{};
    PROCESS_INFORMATION pi{};

    si.cb = sizeof(STARTUPINFOA);
    si.dwFlags |= STARTF_USESTDHANDLES;
    si.hStdOutput = hWrite;
    si.hStdError = hWrite;

    std::string cmdLine = "cmd.exe /C " + command;

    if (!CreateProcessA(
        nullptr,
        cmdLine.data(),
        nullptr,
        nullptr,
        TRUE,
        CREATE_NO_WINDOW,
        nullptr,
        nullptr,
        &si,
        &pi))
    {
        CloseHandle(hWrite);
        CloseHandle(hRead);
        return;
    }

    CloseHandle(hWrite);

    char buffer[4096];
    DWORD bytesRead;

    while (ReadFile(hRead, buffer, sizeof(buffer) - 1, &bytesRead, nullptr)
           && bytesRead > 0)
    {
        buffer[bytesRead] = '\0';

        std::lock_guard<std::mutex> lock(consoleMutex);
        consoleOutput += buffer;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hRead);
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
    }
    else
    {
        std::string cmd = argv[1];

        if(cmd == "help")
        {
            PrintHelp();

            return 0;
        }
        else if(cmd == "-help")
        {
            PrintHelp();

            return 0;
        }
        else if(cmd == "--help")
        {
            PrintHelp();

            return 0;
        }
        else if (cmd == "--version")
        {
            std::cout << currentVersion << std::endl;

            return 0;
        }
        else if (cmd == "--buildEngine")
        {
            std::cout << "Building the Engine\n";

            std::system("buildEngine.bat");

            return 0;
        }
        else if (cmd == "--buildLevelEditor")
        {
            std::cout << "Building the Level Editor\n";

            std::system("buildLevelEditor.bat");

            return 0;
        }
        else if (cmd == "--runEngine")
        {
            std::cout << "Running the Engine\n";

            std::system("runEngine.bat");

            return 0;
        }
        else if (cmd == "--runLevelEditor")
        {
            std::cout << "Running the Level Editor\n";

            std::system("runLevelEditor.bat");

            return 0;
        }
        else
        {
            std::cout << "Unknown command.\n" << std::endl;

            PrintHelp();
        }
    }

    SetTraceLogLevel(LOG_ERROR);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "Builder");

    rlImGuiSetup(true);

    bool showAbout = false;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(DARKGRAY);

        rlImGuiBegin();

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("About"))
            {
                if (ImGui::MenuItem("About"))
                {
                    showAbout = true;
                }

                if (ImGui::MenuItem("Clear Console"))
                {
                    consoleOutput.clear();
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Build"))
            {
                if (ImGui::MenuItem("Build Engine"))
                {
                    std::thread(RunCommand, "buildEngine.bat").detach();
                }
                
                if (ImGui::MenuItem("Build Level Editor"))
                {
                    std::thread(RunCommand, "buildLevelEditor.bat").detach();
                }

                // if (ImGui::MenuItem("Build Compiler"))
                // {
                //     consoleOutput.clear();
                //     std::thread(RunCommand, "buildCompiler.bat").detach();
                // }

                // if (ImGui::MenuItem("Build Level Editor .dll"))
                // {
                //     std::thread(RunCommand, "buildLevelEditordll.bat").detach();
                // }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Run"))
            {
                if (ImGui::MenuItem("Run Engine"))
                {
                    std::thread(RunCommand, "runEngine.bat").detach();
                }

                if (ImGui::MenuItem("Run Level Editor"))
                {
                    std::thread(RunCommand, "runLevelEditor.bat").detach();
                }

                // if (ImGui::MenuItem("Run New Compiler"))
                // {
                //     consoleOutput.clear();
                //     std::thread(RunCommand, "runCompiler.bat").detach();
                    
                //     deInit();

                //     return 0;
                // }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Quit"))
            {
                deInit();
    
                ImGui::EndMenu();
                return 0;
            }

            ImGui::EndMainMenuBar();
        }

        if (showAbout)
        {
            ImGui::Begin("About", &showAbout);

            ImGui::Text("%s\n", currentVersion.c_str());
            ImGui::Text("This is Something I made to make compiling my projects easier! \n");

            ImGui::End();
        }

        ImGui::SetNextWindowPos(
            ImVec2(0, 25),
            ImGuiCond_Always
        );

        ImGui::SetNextWindowSize(
            ImVec2(1280, 695),
            ImGuiCond_Always
        );

        ImGui::Begin(
            "Console",
            nullptr,
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize
        );

        {
            std::lock_guard<std::mutex> lock(consoleMutex);

            // ImGui::BeginChild(
            //     "ConsoleOutput",
            //     ImVec2(0, 0),
            //     true,
            //     ImGuiWindowFlags_HorizontalScrollbar
            // );

            // ImGui::TextUnformatted(consoleOutput.c_str());

            // if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            //     ImGui::SetScrollHereY(1.0f);

            // ImGui::EndChild();

            ImGui::BeginChild(
                "ConsoleOutput",
                ImVec2(0, 0),
                true,
                ImGuiWindowFlags_HorizontalScrollbar
            );

            ImGui::InputTextMultiline(
                "##ConsoleText",
                consoleOutput.data(),
                consoleOutput.size() + 1,
                ImVec2(-1, -1),
                ImGuiInputTextFlags_ReadOnly
            );

            ImGui::EndChild();
        }

        ImGui::End();
        
        rlImGuiEnd();

        EndDrawing();
    }

    deInit();
    return 0;
}