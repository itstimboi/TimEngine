#include "Sprite.h"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>


Sprite::Sprite(const char* texturePath)
    : texture(texturePath, "diffuse", 0),
      VAO(0),
      VBO(0),
      EBO(0)
{
    // --------------------------------------------------------
    // Quad vertices
    // --------------------------------------------------------

    glm::vec2 vertex1 = glm::vec2(-0.5f, -0.5f);
    glm::vec2 vertex2 = glm::vec2( 0.5f, -0.5f);
    glm::vec2 vertex3 = glm::vec2( 0.5f,  0.5f);
    glm::vec2 vertex4 = glm::vec2(-0.5f,  0.5f);

    float vertices[] =
    {
        // Position                 Texture coordinates

        vertex1.x, vertex1.y, 0.0f,  0.0f, 0.0f,
        vertex2.x, vertex2.y, 0.0f,  1.0f, 0.0f,
        vertex3.x, vertex3.y, 0.0f,  1.0f, 1.0f,
        vertex4.x, vertex4.y, 0.0f,  0.0f, 1.0f
    };

    unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };


    // --------------------------------------------------------
    // VAO
    // --------------------------------------------------------

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    // --------------------------------------------------------
    // VBO
    // --------------------------------------------------------

    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );


    // --------------------------------------------------------
    // EBO
    // --------------------------------------------------------

    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    );


    // --------------------------------------------------------
    // Position
    // --------------------------------------------------------

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);


    // --------------------------------------------------------
    // UV
    // --------------------------------------------------------

    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)(3 * sizeof(float))
    );

    glEnableVertexAttribArray(1);


    // --------------------------------------------------------
    // Unbind
    // --------------------------------------------------------

    glBindVertexArray(0);
}


Sprite::~Sprite()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}


void Sprite::Draw(
    glm::vec3 position,
    glm::vec3 scale,
    float rotation,
    Shader& shader,
    Camera& camera
)
{
    shader.Activate();

    glm::mat4 model = glm::mat4(1.0f);

    // Position
    model = glm::translate(
        model,
        position
    );

    // Rotation
    model = glm::rotate(
        model,
        glm::radians(rotation),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    // Scale
    model = glm::scale(
        model,
        scale
    );

    // Send model
    shader.SetMat4(
        "model",
        model
    );

    // Send camera matrix
    camera.Matrix(
        shader,
        "camMatrix"
    );

    // Texture
    texture.Bind();

    shader.SetInt(
        "texture1",
        0
    );

    // Draw
    glBindVertexArray(VAO);

    glDrawElements(
        GL_TRIANGLES,
        6,
        GL_UNSIGNED_INT,
        0
    );

    glBindVertexArray(0);
}