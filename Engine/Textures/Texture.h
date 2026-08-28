#pragma once

#include "glad/glad.h"
#include "stb/stb_image.h"

#include "Rendering/shaderClass.h"

class Texture
{
    public:
        GLuint ID;
        const char* type;
        GLuint unit;
        
        Texture(const char* image, const char* texType, GLenum slot);

        void texUnit(Shader &shader, const char* uniform, GLuint unit);
        void Bind();
        void UnBind();
        void Delete();
};