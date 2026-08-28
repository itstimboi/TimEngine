#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texUV;

    glm::ivec4 boneIDs = glm::ivec4(0);
    glm::vec4 weights = glm::vec4(0.0f);
};

class VBO
{
    public:
        GLuint ID;
        VBO(std::vector <Vertex>& vertices);
        VBO(std::vector <glm::mat4>& mat4s);

        void Bind();
        void Unbind();
        void Delete();
};