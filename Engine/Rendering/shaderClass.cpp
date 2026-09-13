#include "shaderClass.h"

#include "OpenGLShader.h"
#include "Graphics/Graphics.h"

#include <utility>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <cstring>
#include <stdexcept>

namespace TE
{

    struct Shader::Backend
    {
        OpenGLShader* OpenGL = nullptr;
    };


    std::string get_file_contents(
        const char* filename
    )
    {
        errno = 0;

        std::ifstream in(
            filename,
            std::ios::binary
        );

        if (!in.is_open())
        {
            std::cerr
                << "Failed to open shader: "
                << filename
                << '\n';

            std::cerr
                << "errno = "
                << errno
                << '\n';

            std::cerr
                << "message = "
                << std::strerror(errno)
                << '\n';

            throw std::runtime_error(
                std::string("Failed to open file: ")
                + filename
            );
        }

        std::ostringstream contents;

        contents << in.rdbuf();

        return contents.str();
    }


    Shader::Shader(
        GraphicsWindow* window,
        const char* vertexFile,
        const char* fragmentFile,
        ShaderLanguage language
    )
    {
        if (window == nullptr)
        {
            throw std::runtime_error(
                "Shader created with null GraphicsWindow"
            );
        }

        this->window = window;


        //
        // If the caller doesn't specify the language,
        // we'll determine it from the renderer later.
        //

        this->language = language;

        GraphicsRenderer renderer =
            GetGraphicsRenderer(window);


        //
        // OpenGL
        //

        if (renderer.API == Renderer::GL)
        {
            backend = new Backend();

            backend->OpenGL =
                new OpenGLShader(
                    vertexFile,
                    fragmentFile
                );

            return;
        }


        //
        // DirectX
        //

        if (renderer.API == Renderer::DX)
        {
            throw std::runtime_error(
                "DirectX Shader backend is not implemented yet."
            );
        }


        //
        // Vulkan
        //

        if (renderer.API == Renderer::VK)
        {
            throw std::runtime_error(
                "Vulkan Shader backend is not implemented yet."
            );
        }


        //
        // Software
        //

        if (renderer.API == Renderer::Software)
        {
            throw std::runtime_error(
                "Software Shader backend is not implemented yet."
            );


        }

        throw std::runtime_error(
            "Graphics renderer does not support shaders."
        );
    }


    Shader::~Shader()
    {
        Delete();
    }


    TE::Shader::Shader(
        TE::Shader&& other
    ) noexcept
    {
        backend = other.backend;
        window = other.window;
        language = other.language;

        other.backend = nullptr;
        other.window = nullptr;
    }


    TE::Shader& Shader::operator=(
        TE::Shader&& other
    ) noexcept
    {
        if (this != &other)
        {
            Delete();

            backend = other.backend;
            window = other.window;
            language = other.language;

            other.backend = nullptr;
            other.window = nullptr;
        }

        return *this;
    }


    void Shader::Activate()
    {
        if (backend == nullptr)
            return;

        if (backend->OpenGL != nullptr)
        {
            GraphicsMakeCurrent(window);

            backend->OpenGL->Activate();
        }
    }


    void Shader::Delete()
    {
        if (backend == nullptr)
            return;

        if (window != nullptr)
        {
            GraphicsMakeCurrent(window);
        }

        delete backend->OpenGL;
        backend->OpenGL = nullptr;

        delete backend;
        backend = nullptr;
    }


    void Shader::SetBool(
        const std::string& name,
        bool value
    )
    {
        if (backend == nullptr)
            return;

        GraphicsMakeCurrent(window);

        if (backend->OpenGL != nullptr)
        {
            backend->OpenGL->SetBool(
                name,
                value
            );
        }
    }


    void Shader::SetInt(
        const std::string& name,
        int value
    )
    {
        if (backend == nullptr)
            return;

        GraphicsMakeCurrent(window);

        if (backend->OpenGL != nullptr)
        {
            backend->OpenGL->SetInt(
                name,
                value
            );
        }
    }


    void Shader::SetFloat(
        const std::string& name,
        float value
    )
    {
        if (backend == nullptr)
            return;

        GraphicsMakeCurrent(window);

        if (backend->OpenGL != nullptr)
        {
            backend->OpenGL->SetFloat(
                name,
                value
            );
        }
    }


    void Shader::SetVec3(
        const std::string& name,
        const glm::vec3& value
    )
    {
        if (backend == nullptr)
            return;

        GraphicsMakeCurrent(window);

        if (backend->OpenGL != nullptr)
        {
            backend->OpenGL->SetVec3(
                name,
                value
            );
        }
    }


    void Shader::SetVec4(
        const std::string& name,
        const glm::vec4& value
    )
    {
        if (backend == nullptr)
            return;

        GraphicsMakeCurrent(window);

        if (backend->OpenGL != nullptr)
        {
            backend->OpenGL->SetVec4(
                name,
                value
            );
        }
    }


    void Shader::SetMat4(
        const std::string& name,
        const glm::mat4& value
    )
    {
        if (backend == nullptr)
            return;

        GraphicsMakeCurrent(window);

        if (backend->OpenGL != nullptr)
        {
            backend->OpenGL->SetMat4(
                name,
                value
            );
        }
    }


    ShaderLanguage Shader::GetLanguage() const
    {
        return language;
    }


    GraphicsWindow* Shader::GetWindow() const
    {
        return window;
    }

}