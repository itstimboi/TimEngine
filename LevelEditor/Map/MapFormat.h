#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct EditorVertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 uv;
};

struct EditorEdge
{
    unsigned int vertex0 = 0;
    unsigned int vertex1 = 0;
};

struct EditorMaterial
{
    std::string name;
    std::string texture;
};


struct EditorSurface
{
    int material = -1;

    // Edges belonging to this surface.
    std::vector<unsigned int> edges;

    // Triangulation
    std::vector<unsigned int> indices;

    // std::vector<EditorVertex> vertices;

    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
};

struct EditorEntity
{
    unsigned int type;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    bool billboard = true;
    float billboardSize = 1.0f;
};

enum class EditorLightType
{
    Point = 1,
    Spot = 2,
    Directional = 0
};

struct EditorLight
{
    EditorLightType type = EditorLightType::Point;

    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);

    glm::vec3 ambient = glm::vec3(0.2f);
    glm::vec3 diffuse = glm::vec3(1.0f);

    float constant = 1.0f;
    float linear = 0.7f;
    float quadratic = 3.0f;

    float innerCone = 0.95f;
    float outerCone = 0.90f;
};

struct EditorData
{
    int version = 1;

    std::string name;

    std::vector<EditorMaterial> materials;

    std::vector<EditorVertex> vertices;

    std::vector<EditorEdge> edges;

    std::vector<EditorSurface> surfaces;

    std::vector<EditorEntity> entities;

    std::vector<EditorLight> lights;
};