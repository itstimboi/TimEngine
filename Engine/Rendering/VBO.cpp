#include "VAO.h"

VAO::VAO()
{
    glGenVertexArrays(1, &ID);
}

VAO::~VAO()
{
    Delete();
}

void VAO::LinkAttrib(
    VBO& vbo,
    unsigned int layout,
    unsigned int numComponents,
    GLenum type,
    GLsizeiptr stride,
    void* offset
)
{
    Bind();

    vbo.Bind();

    glVertexAttribPointer(
        layout,
        numComponents,
        type,
        GL_FALSE,
        stride,
        offset
    );

    glEnableVertexAttribArray(layout);

    vbo.Unbind();

    Unbind();
}

void VAO::Bind()
{
    glBindVertexArray(ID);
}

void VAO::Unbind()
{
    glBindVertexArray(0);
}

void VAO::Delete()
{
    if (ID != 0)
    {
        glDeleteVertexArrays(1, &ID);
        ID = 0;
    }
}