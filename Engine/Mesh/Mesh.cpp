/*// #include "Mesh.h"

// Mesh::Mesh(
//     std::vector <Vertex>& vertices, 
//     std::vector <unsigned int>& indices, 
//     std::vector <Texture>& textures,
//     unsigned int instancing,
//     std::vector <glm::mat4> instanceMatrix
// )
// {
//     Mesh::vertices = vertices;
//     Mesh::indices = indices;
//     Mesh::textures = textures;
//     Mesh::instancing = instancing;

//     VAO.Bind();

//     VBO instanceVBO(instanceMatrix);
//     VBO VBO(vertices);
//     EBO EBO(indices);

//     VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
//     VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
//     VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, color));
//     VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texUV));

//     if (instancing == 1)
//     {
//         instanceVBO.Bind();

//         // VAO.LinkAttrib(instanceVBO, 4, 4, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
//         // VAO.LinkAttrib(instanceVBO, 5, 4, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
//         // VAO.LinkAttrib(instanceVBO, 6, 4, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, color));
//         // VAO.LinkAttrib(instanceVBO, 7, 4, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texUV));

//         // glVertexAttribDivisor(4, 1);
//         // glVertexAttribDivisor(5, 1);
//         // glVertexAttribDivisor(6, 1);
//         // glVertexAttribDivisor(7, 1);

//         glBindBuffer(GL_ARRAY_BUFFER, instanceVBO.ID);

//         glEnableVertexAttribArray(4);
//         glVertexAttribPointer(
//             4,
//             4,
//             GL_FLOAT,
//             GL_FALSE,
//             sizeof(glm::mat4),
//             (void*)0
//         );

//         glEnableVertexAttribArray(5);
//         glVertexAttribPointer(
//             5,
//             4,
//             GL_FLOAT,
//             GL_FALSE,
//             sizeof(glm::mat4),
//             (void*)(sizeof(glm::vec4))
//         );

//         glEnableVertexAttribArray(6);
//         glVertexAttribPointer(
//             6,
//             4,
//             GL_FLOAT,
//             GL_FALSE,
//             sizeof(glm::mat4),
//             (void*)(2 * sizeof(glm::vec4))
//         );

//         glEnableVertexAttribArray(7);
//         glVertexAttribPointer(
//             7,
//             4,
//             GL_FLOAT,
//             GL_FALSE,
//             sizeof(glm::mat4),
//             (void*)(3 * sizeof(glm::vec4))
//         );

//         glVertexAttribDivisor(4, 1);
//         glVertexAttribDivisor(5, 1);
//         glVertexAttribDivisor(6, 1);
//         glVertexAttribDivisor(7, 1);
//     }

//     VAO.Unbind();
//     VBO.Unbind();
//     instanceVBO.Unbind();
//     EBO.Unbind();
// }

// void Mesh::Draw
// (
// 	Shader& shader, 
// 	Camera& camera,
// 	glm::mat4 matrix,
// 	glm::vec3 translation, 
// 	glm::quat rotation, 
// 	glm::vec3 scale
// )
// {
//     shader.Activate();
//     VAO.Bind();

//     unsigned int numDiffuse = 0;
//     //unsigned int numSpecular = 0;

//     for (unsigned int i = 0; i < textures.size(); i++)
//     {
//         std::string num;
//         std::string type = textures[i].type;
        
//         // if (type == "diffuse")
//         // {
//         //     num = std::to_string(numDiffuse++);
//         // }
//         // else if (type == "specular")
//         // {
//         //     num = std::to_string(numSpecular++);
//         // }
    
//         num = std::to_string(numDiffuse++);

//         textures[i].texUnit(shader, (type + num).c_str(), i);
//         textures[i].Bind();
//     }

//     glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
//     camera.Matrix(shader, "camMatrix");

//     if (instancing == 1)
//     {
//         // Initialize matrices
//         glm::mat4 trans = glm::mat4(1.0f);
//         glm::mat4 rot = glm::mat4(1.0f);
//         glm::mat4 sca = glm::mat4(1.0f);

//         // Transform the matrices to their correct form
//         trans = glm::translate(trans, translation);
//         rot = glm::mat4_cast(rotation);
//         sca = glm::scale(sca, scale);

//         // Push the matrices to the vertex shader
//         glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
//         glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
//         glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
//         glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

//         glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
//     }
//     else
//     {
//         glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, instancing);
//     }
// }*/

#include "Mesh.h"

namespace TE
{

    Mesh::Mesh(
        std::vector<Vertex>& vertices,
        std::vector<unsigned int>& indices,
        std::vector<Texture>& textures,
        unsigned int instancing,
        std::vector<glm::mat4> instanceMatrix
    )
    {
        Mesh::vertices = vertices;
        Mesh::indices = indices;
        Mesh::textures = textures;
        Mesh::instancing = instancing;

        VAO.Bind();

        VBO instanceVBO(instanceMatrix);
        VBO vertexVBO(vertices);
        EBO indexEBO(indices);

        VAO.LinkAttrib(
            vertexVBO,
            0,
            3,
            GL_FLOAT,
            sizeof(Vertex),
            (void*)offsetof(Vertex, position)
        );

        VAO.LinkAttrib(
            vertexVBO,
            1,
            3,
            GL_FLOAT,
            sizeof(Vertex),
            (void*)offsetof(Vertex, normal)
        );

        VAO.LinkAttrib(
            vertexVBO,
            2,
            3,
            GL_FLOAT,
            sizeof(Vertex),
            (void*)offsetof(Vertex, color)
        );

        VAO.LinkAttrib(
            vertexVBO,
            3,
            2,
            GL_FLOAT,
            sizeof(Vertex),
            (void*)offsetof(Vertex, texUV)
        );

        if (instancing != 1)
        {
            instanceVBO.Bind();

            glBindBuffer(GL_ARRAY_BUFFER, instanceVBO.ID);

            glEnableVertexAttribArray(4);
            glVertexAttribPointer(
                4,
                4,
                GL_FLOAT,
                GL_FALSE,
                sizeof(glm::mat4),
                (void*)0
            );

            glEnableVertexAttribArray(5);
            glVertexAttribPointer(
                5,
                4,
                GL_FLOAT,
                GL_FALSE,
                sizeof(glm::mat4),
                (void*)(sizeof(glm::vec4))
            );

            glEnableVertexAttribArray(6);
            glVertexAttribPointer(
                6,
                4,
                GL_FLOAT,
                GL_FALSE,
                sizeof(glm::mat4),
                (void*)(2 * sizeof(glm::vec4))
            );

            glEnableVertexAttribArray(7);
            glVertexAttribPointer(
                7,
                4,
                GL_FLOAT,
                GL_FALSE,
                sizeof(glm::mat4),
                (void*)(3 * sizeof(glm::vec4))
            );

            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);
            glVertexAttribDivisor(7, 1);
        }

        VAO.Unbind();
        vertexVBO.Unbind();
        instanceVBO.Unbind();
        indexEBO.Unbind();
    }


    void Mesh::Draw(
        TE::Shader& shader,
        Camera& camera,
        glm::mat4 matrix,
        glm::vec3 translation,
        glm::quat rotation,
        glm::vec3 scale
    )
    {
        shader.Activate();
        VAO.Bind();

        unsigned int numDiffuse = 0;

        for (unsigned int i = 0; i < textures.size(); i++)
        {
            std::string num;
            std::string type = textures[i].type;

            num = std::to_string(numDiffuse++);

            textures[i].texUnit(
                shader,
                (type + num).c_str(),
                i
            );

            textures[i].Bind();
        }

        shader.SetVec3(
            "camPos",
            (glm::vec3){
            camera.Position.x,
            camera.Position.y,
            camera.Position.z}
        );

        camera.Matrix(shader, "camMatrix");

        if (instancing == 1)
        {
            glm::mat4 trans = glm::translate(
                glm::mat4(1.0f),
                translation
            );

            glm::mat4 rot = glm::mat4_cast(rotation);

            glm::mat4 sca = glm::scale(
                glm::mat4(1.0f),
                scale
            );

            shader.SetMat4("translation", trans);

            shader.SetMat4("rotation", rot);

            shader.SetMat4("scale", sca);

            shader.SetMat4("model", matrix);

            glDrawElements(
                GL_TRIANGLES,
                static_cast<GLsizei>(indices.size()),
                GL_UNSIGNED_INT,
                nullptr
            );
        }
        else
        {
            glDrawElementsInstanced(
                GL_TRIANGLES,
                static_cast<GLsizei>(indices.size()),
                GL_UNSIGNED_INT,
                nullptr,
                instancing
            );
        }
    }

}