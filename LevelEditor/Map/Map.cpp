#include "Map.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <cfloat>

#include "json/json.hpp"

#include "../FileDialog.h"

using json = nlohmann::json;


// ============================================================
// JSON helpers
// ============================================================


// ------------------------------------------------------------
// Material -> JSON
// ------------------------------------------------------------

static json MaterialToJSON(
    const EditorMaterial& material
)
{
    return
    {
        {
            "name",
            material.name
        },

        {
            "texture",
            material.texture
        }
    };
}


// ------------------------------------------------------------
// JSON -> Material
// ------------------------------------------------------------

static EditorMaterial MaterialFromJSON(
    const json& jsonMaterial
)
{
    EditorMaterial material;

    material.name =
        jsonMaterial.value(
            "name",
            "Unnamed Material"
        );

    material.texture =
        jsonMaterial.value(
            "texture",
            ""
        );

    return material;
}


// ------------------------------------------------------------
// Vertex -> JSON
// ------------------------------------------------------------

static json VertexToJSON(
    const EditorVertex& vertex
)
{
    return
    {
        {
            "position",
            {
                vertex.position.x,
                vertex.position.y,
                vertex.position.z
            }
        },

        {
            "normal",
            {
                vertex.normal.x,
                vertex.normal.y,
                vertex.normal.z
            }
        },

        {
            "color",
            {
                vertex.color.x,
                vertex.color.y,
                vertex.color.z
            }
        },

        {
            "uv",
            {
                vertex.uv.x,
                vertex.uv.y
            }
        }
    };
}

static json LightToJSON(const EditorLight& light)
{
    return
    {
        {
            "type",
            static_cast<int>(light.type)
        },

        {
            "position",
            {
                light.position.x,
                light.position.y,
                light.position.z
            }
        },

        {
            "direction",
            {
                light.direction.x,
                light.direction.y,
                light.direction.z
            }
        },

        {
            "ambient",
            {
                light.ambient.r,
                light.ambient.g,
                light.ambient.b
            }
        },

        {
            "diffuse",
            {
                light.diffuse.r,
                light.diffuse.g,
                light.diffuse.b
            }
        },

        {
            "constant",
            light.constant
        },

        {
            "linear",
            light.linear
        },

        {
            "quadratic",
            light.quadratic
        },

        {
            "innerCone",
            light.innerCone
        },

        {
            "outerCone",
            light.outerCone
        }
    };
}

// ------------------------------------------------------------
// JSON -> Vertex
// ------------------------------------------------------------

static EditorVertex VertexFromJSON(
    const json& jsonVertex
)
{
    EditorVertex vertex;

    const auto& position =
        jsonVertex["position"];

    vertex.position =
        glm::vec3(
            position[0],
            position[1],
            position[2]
        );


    const auto& normal =
        jsonVertex["normal"];

    vertex.normal =
        glm::vec3(
            normal[0],
            normal[1],
            normal[2]
        );


    const auto& color =
        jsonVertex["color"];

    vertex.color =
        glm::vec3(
            color[0],
            color[1],
            color[2]
        );


    const auto& uv =
        jsonVertex["uv"];

    vertex.uv =
        glm::vec2(
            uv[0],
            uv[1]
        );


    return vertex;
}


// ------------------------------------------------------------
// Edge -> JSON
// ------------------------------------------------------------

static json EdgeToJSON(
    const EditorEdge& edge
)
{
    return
    {
        {
            "vertexA",
            edge.vertex0
        },

        {
            "vertexB",
            edge.vertex1
        }
    };
}


// ------------------------------------------------------------
// JSON -> Edge
// ------------------------------------------------------------

static EditorEdge EdgeFromJSON(
    const json& jsonEdge
)
{
    EditorEdge edge;

    edge.vertex0 =
        jsonEdge.value(
            "vertexA",
            0
        );

    edge.vertex1 =
        jsonEdge.value(
            "vertexB",
            0
        );

    return edge;
}


// ------------------------------------------------------------
// Surface -> JSON
// ------------------------------------------------------------

static json SurfaceToJSON(
    const EditorSurface& surface
)
{
    return
    {
        {
            "material",
            surface.material
        },

        {
            "edges",
            surface.edges
        },

        {
            "rotation",
            surface.rotation
        },

        {
            "scale",
            surface.scale
        }
    };
}


// ------------------------------------------------------------
// JSON -> Surface
// ------------------------------------------------------------

static EditorSurface SurfaceFromJSON(
    const json& jsonSurface
)
{
    EditorSurface surface;

    surface.material =
        jsonSurface.value(
            "material",
            0
        );

    if (jsonSurface.contains("edges"))
    {
        for (const auto& edge :
             jsonSurface["edges"])
        {
            surface.edges.push_back(
                edge.get<unsigned int>()
            );
        }
    }

    if (jsonSurface.contains("rotation"))
    {
        auto surfaceRotation = jsonSurface["rotation"];

        glm::vec3 returnValue;

        returnValue.x = surfaceRotation[0].get<float>();
        returnValue.y = surfaceRotation[1].get<float>();
        returnValue.z = surfaceRotation[2].get<float>();

        surface.rotation = returnValue;
    }

    if (jsonSurface.contains("scale"))
    {
        auto surfaceScale = jsonSurface["scale"];

        glm::vec3 returnValue;
        returnValue.x = surfaceScale[0].get<float>();
        returnValue.y = surfaceScale[1].get<float>();
        returnValue.z = surfaceScale[2].get<float>();

        surface.scale = returnValue;
    }

    return surface;
}


// ------------------------------------------------------------
// Entity -> JSON
// ------------------------------------------------------------

static json EntityToJSON(
    const EditorEntity& entity
)
{
    return
    {
        {
            "type",
            entity.type
        },

        {
            "position",
            {
                entity.position.x,
                entity.position.y,
                entity.position.z
            }
        },

        {
            "rotation",
            {
                entity.rotation.x,
                entity.rotation.y,
                entity.rotation.z
            }
        },

        {
            "scale",
            {
                entity.scale.x,
                entity.scale.y,
                entity.scale.z
            }
        }
    };
}


// ------------------------------------------------------------
// JSON -> Entity
// ------------------------------------------------------------

static EditorEntity EntityFromJSON(
    const json& jsonEntity
)
{
    EditorEntity entity;

    entity.type =
        jsonEntity.value(
            "type",
            0u
        );


    const auto& position =
        jsonEntity["position"];

    entity.position =
        glm::vec3(
            position[0],
            position[1],
            position[2]
        );


    const auto& rotation =
        jsonEntity["rotation"];

    entity.rotation =
        glm::vec3(
            rotation[0],
            rotation[1],
            rotation[2]
        );


    const auto& scale =
        jsonEntity["scale"];

    entity.scale =
        glm::vec3(
            scale[0],
            scale[1],
            scale[2]
        );


    return entity;
}


// ============================================================
// Constructor
// ============================================================

Map::Map()
    : selectedSurface(-1),
      hoveredSurface(-1),
      selectedVertex(-1),
      hoveredVertex(-1),
      selectedEdge(-1),
      hoveredEdge(-1),
      selectedEntity(-1),
      hoveredEntity(-1),
      selectedLight(-1),
      hoveredLight(-1)
{
    New();
}


// ============================================================
// Destructor
// ============================================================

Map::~Map()
{
    for (auto& mesh : renderMeshes)
    {
        UnloadMesh(mesh);
    }

    renderMeshes.clear();

    renderMaterials.clear();

    UnloadTextures();
}


// ============================================================
// New
// ============================================================

void Map::New(
    const std::string& name
)
{
    for (auto& mesh : renderMeshes)
    {
        UnloadMesh(mesh);
    }

    renderMeshes.clear();

    renderMaterials.clear();

    UnloadTextures();


    mapData =
        EditorData{};

    mapData.version =
        1;

    mapData.name =
        name;


    undoStack.clear();


    selectedSurface =
        -1;

    hoveredSurface =
        -1;

    selectedVertex =
        -1;

    hoveredVertex =
        -1;

    selectedEdge =
        -1;

    hoveredEdge =
        -1;

    selectedEntity =
        -1;

    hoveredEntity =
        -1;

    selectedLight = -1;
    hoveredLight = -1;
}


// ============================================================
// Save
// ============================================================

bool Map::Save(
    const std::string& filename
) const
{
    json map;


    map["version"] =
        mapData.version;

    map["name"] =
        mapData.name;


    // --------------------------------------------------------
    // Materials
    // --------------------------------------------------------

    map["materials"] =
        json::array();

    for (const EditorMaterial& material :
         mapData.materials)
    {
        map["materials"].push_back(
            MaterialToJSON(material)
        );
    }


    // --------------------------------------------------------
    // Vertices
    // --------------------------------------------------------

    map["vertices"] =
        json::array();

    for (const EditorVertex& vertex :
         mapData.vertices)
    {
        map["vertices"].push_back(
            VertexToJSON(vertex)
        );
    }


    // --------------------------------------------------------
    // Edges
    // --------------------------------------------------------

    map["edges"] =
        json::array();

    for (const EditorEdge& edge :
         mapData.edges)
    {
        map["edges"].push_back(
            EdgeToJSON(edge)
        );
    }


    // --------------------------------------------------------
    // Surfaces
    // --------------------------------------------------------

    map["surfaces"] =
        json::array();

    for (const EditorSurface& surface :
         mapData.surfaces)
    {
        map["surfaces"].push_back(
            SurfaceToJSON(surface)
        );
    }


    // --------------------------------------------------------
    // Entities
    // --------------------------------------------------------

    map["entities"] =
        json::array();

    for (const EditorEntity& entity :
         mapData.entities)
    {
        map["entities"].push_back(
            EntityToJSON(entity)
        );
    }

    map["lights"] = json::array();

    for (const EditorLight& light : mapData.lights)
    {
        map["lights"].push_back(
            LightToJSON(light)
        );
    }

    // --------------------------------------------------------
    // Write
    // --------------------------------------------------------

    std::ofstream file(
        filename
    );

    if (!file.is_open())
    {
        std::cerr
            << "Failed to save map: "
            << filename
            << '\n';

        return false;
    }


    file << map.dump(4);

    file.close();


    std::cout
        << "Saved map: "
        << filename
        << '\n';


    return true;
}


// ============================================================
// Load
// ============================================================

bool Map::Load(
    const std::string& filename
)
{
    std::ifstream file(
        filename
    );

    if (!file.is_open())
    {
        std::cerr
            << "Failed to open map: "
            << filename
            << '\n';

        return false;
    }


    json map;


    try
    {
        file >> map;
    }
    catch (const json::parse_error& error)
    {
        std::cerr
            << "Failed to parse map: "
            << error.what()
            << '\n';

        return false;
    }


    // --------------------------------------------------------
    // Version
    // --------------------------------------------------------

    if (!map.contains("version"))
    {
        std::cerr
            << "Map does not contain a version!\n";

        return false;
    }


    mapData.version =
        map["version"];


    // --------------------------------------------------------
    // Name
    // --------------------------------------------------------

    if (map.contains("name"))
    {
        mapData.name =
            map["name"];
    }
    else
    {
        mapData.name =
            "Untitled Map";
    }


    // --------------------------------------------------------
    // Clear
    // --------------------------------------------------------

    mapData.materials.clear();
    mapData.vertices.clear();
    mapData.edges.clear();
    mapData.surfaces.clear();
    mapData.entities.clear();


    // --------------------------------------------------------
    // Materials
    // --------------------------------------------------------

    if (map.contains("materials"))
    {
        for (const auto& jsonMaterial :
             map["materials"])
        {
            mapData.materials.push_back(
                MaterialFromJSON(
                    jsonMaterial
                )
            );
        }
    }


    // --------------------------------------------------------
    // Textures
    // --------------------------------------------------------

    UnloadTextures();


    for (const EditorMaterial& material :
         mapData.materials)
    {
        if (material.texture.empty())
        {
            textures.push_back(
                Texture2D{}
            );

            continue;
        }


        std::filesystem::path texturePath =
            GetProjectRoot()
            /
            GetParentFolderName()
            /
            material.texture;


        std::cout
            << "Loading texture: "
            << texturePath.string()
            << '\n';


        if (!std::filesystem::exists(texturePath))
        {
            std::cerr
                << "TEXTURE NOT FOUND: "
                << texturePath.string()
                << '\n';

            textures.push_back(
                Texture2D{}
            );

            continue;
        }


        Texture2D texture =
            LoadTexture(
                texturePath.string().c_str()
            );


        if (texture.id == 0)
        {
            std::cerr
                << "FAILED TO LOAD TEXTURE: "
                << material.texture
                << '\n';
        }
        else
        {
            std::cout
                << "Loaded texture: "
                << material.texture
                << " ("
                << texture.width
                << "x"
                << texture.height
                << ")\n";
        }


        textures.push_back(
            texture
        );
    }


    // --------------------------------------------------------
    // Vertices
    // --------------------------------------------------------

    if (map.contains("vertices"))
    {
        for (const auto& jsonVertex :
             map["vertices"])
        {
            mapData.vertices.push_back(
                VertexFromJSON(
                    jsonVertex
                )
            );
        }
    }


    // --------------------------------------------------------
    // Edges
    // --------------------------------------------------------

    if (map.contains("edges"))
    {
        for (const auto& jsonEdge :
             map["edges"])
        {
            mapData.edges.push_back(
                EdgeFromJSON(
                    jsonEdge
                )
            );
        }
    }


    // --------------------------------------------------------
    // Surfaces
    // --------------------------------------------------------

    if (map.contains("surfaces"))
    {
        for (const auto& jsonSurface :
             map["surfaces"])
        {
            mapData.surfaces.push_back(
                SurfaceFromJSON(
                    jsonSurface
                )
            );
        }
    }


    // --------------------------------------------------------
    // Entities
    // --------------------------------------------------------

    if (map.contains("entities"))
    {
        for (const auto& jsonEntity :
             map["entities"])
        {
            mapData.entities.push_back(
                EntityFromJSON(
                    jsonEntity
                )
            );
        }
    }

    if (map.contains("lights"))
    {
        mapData.lights.clear();

        for (const auto& lightJSON : map["lights"])
        {
            EditorLight light;

            light.type =
                static_cast<EditorLightType>(
                    lightJSON.value("type", 0)
                );

            auto position =
                lightJSON.value(
                    "position",
                    std::vector<float>{0.0f, 0.0f, 0.0f}
                );

            if (position.size() >= 3)
            {
                light.position =
                    glm::vec3(
                        position[0],
                        position[1],
                        position[2]
                    );
            }

            auto direction =
                lightJSON.value(
                    "direction",
                    std::vector<float>{0.0f, -1.0f, 0.0f}
                );

            if (direction.size() >= 3)
            {
                light.direction =
                    glm::vec3(
                        direction[0],
                        direction[1],
                        direction[2]
                    );
            }

            auto ambient =
                lightJSON.value(
                    "ambient",
                    std::vector<float>{0.2f, 0.2f, 0.2f}
                );

            if (ambient.size() >= 3)
            {
                light.ambient =
                    glm::vec3(
                        ambient[0],
                        ambient[1],
                        ambient[2]
                    );
            }

            auto diffuse =
                lightJSON.value(
                    "diffuse",
                    std::vector<float>{1.0f, 1.0f, 1.0f}
                );

            if (diffuse.size() >= 3)
            {
                light.diffuse =
                    glm::vec3(
                        diffuse[0],
                        diffuse[1],
                        diffuse[2]
                    );
            }

            light.constant =
                lightJSON.value("constant", 1.0f);

            light.linear =
                lightJSON.value("linear", 0.7f);

            light.quadratic =
                lightJSON.value("quadratic", 3.0f);

            light.innerCone =
                lightJSON.value("innerCone", 0.95f);

            light.outerCone =
                lightJSON.value("outerCone", 0.90f);

            mapData.lights.push_back(light);
        }
    }


    std::cout
        << "Loaded map: "
        << filename
        << '\n';


    std::cout
        << "Materials: "
        << mapData.materials.size()
        << '\n';


    std::cout
        << "Vertices: "
        << mapData.vertices.size()
        << '\n';


    std::cout
        << "Edges: "
        << mapData.edges.size()
        << '\n';


    std::cout
        << "Surfaces: "
        << mapData.surfaces.size()
        << '\n';


    std::cout
        << "Entities: "
        << mapData.entities.size()
        << '\n';


    if (!BuildRenderMeshes())
    {
        std::cerr
            << "Failed to build render meshes.\n";

        return false;
    }


    selectedSurface =
        -1;

    hoveredSurface =
        -1;

    selectedVertex =
        -1;

    hoveredVertex =
        -1;

    selectedEdge = 
        -1;
        
    hoveredEdge = 
        -1;

    selectedEntity = -1;
    hoveredEntity = -1;

    selectedLight = -1;
    hoveredLight = -1;

    return true;
}


// ============================================================
// GetData
// ============================================================

EditorData& Map::GetData()
{
    return mapData;
}


const EditorData& Map::GetData() const
{
    return mapData;
}


// ============================================================
// AddEdge
// ============================================================

unsigned int Map::AddEdge(
    unsigned int vertexA,
    unsigned int vertexB
)
{
    EditorData& mapData = GetData();

    // --------------------------------------------------------
    // Check that the vertices actually exist
    // --------------------------------------------------------

    if (vertexA >= mapData.vertices.size() ||
        vertexB >= mapData.vertices.size())
    {
        std::cerr
            << "Cannot create edge. Invalid vertex.\n";

        return UINT_MAX;
    }


    // --------------------------------------------------------
    // Don't create duplicate edges
    // --------------------------------------------------------

    for (unsigned int i = 0;
         i < mapData.edges.size();
         i++)
    {
        const EditorEdge& edge =
            mapData.edges[i];

        if ((edge.vertex0 == vertexA &&
             edge.vertex1 == vertexB) ||

            (edge.vertex0 == vertexB &&
             edge.vertex1 == vertexA))
        {
            return i;
        }
    }


    // --------------------------------------------------------
    // Create edge
    // --------------------------------------------------------

    EditorEdge edge;

    edge.vertex0 = vertexA;
    edge.vertex1 = vertexB;

    mapData.edges.push_back(edge);


    return static_cast<unsigned int>(
        mapData.edges.size() - 1
    );
}

// ============================================================
// GetSurfaceVertices
// ============================================================

std::vector<unsigned int>
Map::GetSurfaceVertices(
    const EditorSurface& surface,
    const EditorData& data
) const
{
    std::vector<unsigned int> vertices;

    for (unsigned int edgeIndex : surface.edges)
    {
        if (edgeIndex >= data.edges.size())
            continue;

        const EditorEdge& edge =
            data.edges[edgeIndex];

        std::vector<unsigned int> edgeVertices =
            GetEdgeVertices(edge, data);

        for (unsigned int vertexIndex : edgeVertices)
        {
            if (std::find(
                    vertices.begin(),
                    vertices.end(),
                    vertexIndex
                ) == vertices.end())
            {
                vertices.push_back(vertexIndex);
            }
        }
    }

    return vertices;
}
// ============================================================
// AddCube
// ============================================================

void Map::AddCube(
    const glm::vec3& min,
    const glm::vec3& max
)
{
    unsigned int start =
        static_cast<unsigned int>(
            mapData.vertices.size()
        );


    // ========================================================
    // Create vertices
    // ========================================================


    // --------------------------------------------------------
    // Bottom
    // --------------------------------------------------------

    mapData.vertices.push_back({
        { min.x, min.y, min.z },
        { 0.0f, -1.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f },
        { 0.0f, 0.0f }
    });

    mapData.vertices.push_back({
        { max.x, min.y, min.z },
        { 0.0f, -1.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f },
        { 1.0f, 0.0f }
    });

    mapData.vertices.push_back({
        { max.x, min.y, max.z },
        { 0.0f, -1.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f }
    });

    mapData.vertices.push_back({
        { min.x, min.y, max.z },
        { 0.0f, -1.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f },
        { 0.0f, 1.0f }
    });


    // --------------------------------------------------------
    // Top
    // --------------------------------------------------------

    mapData.vertices.push_back({
        { min.x, max.y, min.z },
        { 0.0f, 1.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f },
        { 0.0f, 0.0f }
    });

    mapData.vertices.push_back({
        { max.x, max.y, min.z },
        { 0.0f, 1.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f },
        { 1.0f, 0.0f }
    });

    mapData.vertices.push_back({
        { max.x, max.y, max.z },
        { 0.0f, 1.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f }
    });

    mapData.vertices.push_back({
        { min.x, max.y, max.z },
        { 0.0f, 1.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f },
        { 0.0f, 1.0f }
    });


    // --------------------------------------------------------
    // Front
    // --------------------------------------------------------

    mapData.vertices.push_back({
        { min.x, min.y, max.z },
        { 0.0f, 0.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f },
        { 0.0f, 0.0f }
    });

    mapData.vertices.push_back({
        { max.x, min.y, max.z },
        { 0.0f, 0.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f },
        { 1.0f, 0.0f }
    });

    mapData.vertices.push_back({
        { max.x, max.y, max.z },
        { 0.0f, 0.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f }
    });

    mapData.vertices.push_back({
        { min.x, max.y, max.z },
        { 0.0f, 0.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f },
        { 0.0f, 1.0f }
    });


    // --------------------------------------------------------
    // Back
    // --------------------------------------------------------

    mapData.vertices.push_back({
        { max.x, min.y, min.z },
        { 0.0f, 0.0f, -1.0f },
        { 1.0f, 1.0f, 1.0f },
        { 0.0f, 0.0f }
    });

    mapData.vertices.push_back({
        { min.x, min.y, min.z },
        { 0.0f, 0.0f, -1.0f },
        { 1.0f, 1.0f, 1.0f },
        { 1.0f, 0.0f }
    });

    mapData.vertices.push_back({
        { min.x, max.y, min.z },
        { 0.0f, 0.0f, -1.0f },
        { 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f }
    });

    mapData.vertices.push_back({
        { max.x, max.y, min.z },
        { 0.0f, 0.0f, -1.0f },
        { 1.0f, 1.0f, 1.0f },
        { 0.0f, 1.0f }
    });


    // --------------------------------------------------------
    // Left
    // --------------------------------------------------------

    mapData.vertices.push_back({
        { min.x, min.y, min.z },
        { -1.0f, 0.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f },
        { 0.0f, 0.0f }
    });

    mapData.vertices.push_back({
        { min.x, min.y, max.z },
        { -1.0f, 0.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f },
        { 1.0f, 0.0f }
    });

    mapData.vertices.push_back({
        { min.x, max.y, max.z },
        { -1.0f, 0.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f }
    });

    mapData.vertices.push_back({
        { min.x, max.y, min.z },
        { -1.0f, 0.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f },
        { 0.0f, 1.0f }
    });


    // --------------------------------------------------------
    // Right
    // --------------------------------------------------------

    mapData.vertices.push_back({
        { max.x, min.y, max.z },
        { 1.0f, 0.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f },
        { 0.0f, 0.0f }
    });

    mapData.vertices.push_back({
        { max.x, min.y, min.z },
        { 1.0f, 0.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f },
        { 1.0f, 0.0f }
    });

    mapData.vertices.push_back({
        { max.x, max.y, min.z },
        { 1.0f, 0.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f }
    });

    mapData.vertices.push_back({
        { max.x, max.y, max.z },
        { 1.0f, 0.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f },
        { 0.0f, 1.0f }
    });


    // ========================================================
    // Create surfaces from edges
    // ========================================================

    auto CreateSurface =
        [this](
            unsigned int v0,
            unsigned int v1,
            unsigned int v2,
            unsigned int v3
        )
    {
        EditorSurface surface;

        surface.material =
            0;


        unsigned int e0 =
            AddEdge(v0, v1);

        unsigned int e1 =
            AddEdge(v1, v2);

        unsigned int e2 =
            AddEdge(v2, v3);

        unsigned int e3 =
            AddEdge(v3, v0);


        surface.edges =
        {
            e0,
            e1,
            e2,
            e3
        };


        mapData.surfaces.push_back(
            surface
        );
    };


    // Bottom
    CreateSurface(
        start + 0,
        start + 1,
        start + 2,
        start + 3
    );


    // Top
    CreateSurface(
        start + 4,
        start + 5,
        start + 6,
        start + 7
    );


    // Front
    CreateSurface(
        start + 8,
        start + 9,
        start + 10,
        start + 11
    );


    // Back
    CreateSurface(
        start + 12,
        start + 13,
        start + 14,
        start + 15
    );


    // Left
    CreateSurface(
        start + 16,
        start + 17,
        start + 18,
        start + 19
    );


    // Right
    CreateSurface(
        start + 20,
        start + 21,
        start + 22,
        start + 23
    );


    BuildRenderMeshes();
}


// ============================================================
// Draw
// ============================================================

void Map::Draw()
{
    for (size_t i = 0;
         i < renderMeshes.size();
         i++)
    {
        if (i >= renderMaterials.size())
            continue;


        DrawMesh(
            renderMeshes[i],
            renderMaterials[i],
            MatrixIdentity()
        );
    }
}


// ============================================================
// BuildRenderMeshes
// ============================================================

bool Map::BuildRenderMeshes()
{
    // --------------------------------------------------------
    // Delete old meshes
    // --------------------------------------------------------

    for (auto& mesh :
         renderMeshes)
    {
        UnloadMesh(mesh);
    }


    renderMeshes.clear();

    renderMaterials.clear();


    // --------------------------------------------------------
    // Build one mesh per surface
    // --------------------------------------------------------

    for (const EditorSurface& surface :
         mapData.surfaces)
    {
        std::vector<unsigned int>
            surfaceVertices =
                GetSurfaceVertices(surface, mapData);


        if (surfaceVertices.size() < 3)
            continue;


        const size_t vertexCount =
            surfaceVertices.size();


        const size_t indexCount =
            (vertexCount - 2) * 3;


        Mesh mesh = {};


        mesh.vertexCount =
            static_cast<int>(
                vertexCount
            );


        mesh.triangleCount =
            static_cast<int>(
                vertexCount - 2
            );


        // ----------------------------------------------------
        // Allocate vertices
        // ----------------------------------------------------

        mesh.vertices =
            (float*)MemAlloc(
                vertexCount *
                3 *
                sizeof(float)
            );


        mesh.normals =
            (float*)MemAlloc(
                vertexCount *
                3 *
                sizeof(float)
            );


        mesh.texcoords =
            (float*)MemAlloc(
                vertexCount *
                2 *
                sizeof(float)
            );


        mesh.indices =
            (unsigned short*)MemAlloc(
                indexCount *
                sizeof(unsigned short)
            );


        // ----------------------------------------------------
        // Copy vertices
        // ----------------------------------------------------

        for (size_t i = 0;
             i < vertexCount;
             i++)
        {
            unsigned int globalIndex =
                surfaceVertices[i];


            if (globalIndex >=
                mapData.vertices.size())
            {
                std::cerr
                    << "Invalid vertex index: "
                    << globalIndex
                    << '\n';

                continue;
            }


            const EditorVertex& vertex =
                mapData.vertices[globalIndex];


            // Position
            mesh.vertices[i * 3 + 0] =
                vertex.position.x;

            mesh.vertices[i * 3 + 1] =
                vertex.position.y;

            mesh.vertices[i * 3 + 2] =
                vertex.position.z;


            // Normal
            mesh.normals[i * 3 + 0] =
                vertex.normal.x;

            mesh.normals[i * 3 + 1] =
                vertex.normal.y;

            mesh.normals[i * 3 + 2] =
                vertex.normal.z;


            // UV
            mesh.texcoords[i * 2 + 0] =
                vertex.uv.x;

            mesh.texcoords[i * 2 + 1] =
                vertex.uv.y;
        }


        // ----------------------------------------------------
        // Generate triangle fan
        // ----------------------------------------------------

        size_t index = 0;


        for (size_t i = 1;
             i + 1 < vertexCount;
             i++)
        {
            mesh.indices[index++] =
                0;

            mesh.indices[index++] =
                static_cast<unsigned short>(
                    i
                );

            mesh.indices[index++] =
                static_cast<unsigned short>(
                    i + 1
                );
        }


        // ----------------------------------------------------
        // Upload
        // ----------------------------------------------------

        UploadMesh(
            &mesh,
            false
        );


        renderMeshes.push_back(
            mesh
        );


        // ----------------------------------------------------
        // Material
        // ----------------------------------------------------

        Material material =
            LoadMaterialDefault();


        if (surface.material >= 0 &&
            surface.material <
                static_cast<int>(textures.size()))
        {
            Texture2D& texture =
                textures[surface.material];


            if (texture.id != 0)
            {
                SetMaterialTexture(
                    &material,
                    MATERIAL_MAP_DIFFUSE,
                    texture
                );
            }
        }


        renderMaterials.push_back(
            material
        );
    }


    return true;
}


// ============================================================
// UnloadTextures
// ============================================================

void Map::UnloadTextures()
{
    for (Texture2D& texture :
         textures)
    {
        if (texture.id != 0)
        {
            UnloadTexture(texture);
        }
    }


    textures.clear();
}

// ============================================================
// Selection
// ============================================================

int Map::GetSelectedSurface() const
{
    return selectedSurface;
}


void Map::SetSelectedSurface(int index)
{
    if (index < -1 ||
        index >= static_cast<int>(mapData.surfaces.size()))
    {
        selectedSurface = -1;
        return;
    }

    selectedSurface = index;
}


int Map::GetHoveredSurface() const
{
    return hoveredSurface;
}


void Map::SetHoveredSurface(int index)
{
    if (index < -1 ||
        index >= static_cast<int>(mapData.surfaces.size()))
    {
        hoveredSurface = -1;
        return;
    }

    hoveredSurface = index;
}

int Map::GetHoveredLight() const
{
    return hoveredLight;
}

void Map::SetHoveredLight(int index)
{
    if (index < -1 ||
        index >= static_cast<int>(mapData.lights.size()))
    {
        hoveredLight = -1;
        return;
    }

    hoveredLight = index;
}

int Map::GetSelectedLight() const
{
    return hoveredLight;
}

void Map::SetSelectedLight(int index)
{
    if (index < -1 ||
        index >= static_cast<int>(mapData.lights.size()))
    {
        selectedLight = -1;
        return;
    }

    selectedLight = index;
}


int Map::GetSelectedEntity() const
{
    return selectedEntity;
}


void Map::SetSelectedEntity(int index)
{
    if (index < -1 ||
        index >= static_cast<int>(mapData.entities.size()))
    {
        selectedEntity = -1;
        return;
    }

    selectedEntity = index;
}


int Map::GetHoveredEntity() const
{
    return hoveredEntity;
}


void Map::SetHoveredEntity(int index)
{
    if (index < -1 ||
        index >= static_cast<int>(mapData.entities.size()))
    {
        hoveredEntity = -1;
        return;
    }

    hoveredEntity = index;
}


// ============================================================
// Vertex selection
// ============================================================

int Map::GetSelectedVertex() const
{
    return selectedVertex;
}

EditorVertex Map::GetVertex(int index, const EditorData& data)
{
    return data.vertices[index];
}

EditorSurface Map::GetSurface(int index, const EditorData& data)
{
    return data.surfaces[index];
}

void Map::SetSelectedVertex(int index)
{
    if (index < -1 ||
        index >= static_cast<int>(mapData.vertices.size()))
    {
        selectedVertex = -1;
        return;
    }

    selectedVertex = index;
}


int Map::GetHoveredVertex() const
{
    return hoveredVertex;
}


void Map::SetHoveredVertex(int index)
{
    if (index < -1 ||
        index >= static_cast<int>(mapData.vertices.size()))
    {
        hoveredVertex = -1;
        return;
    }

    hoveredVertex = index;
}


// ============================================================
// Edge selection
// ============================================================

int Map::GetSelectedEdge() const
{
    return selectedEdge;
}


void Map::SetSelectedEdge(int index)
{
    if (index < -1 ||
        index >= static_cast<int>(mapData.edges.size()))
    {
        selectedEdge = -1;
        return;
    }

    selectedEdge = index;
}


int Map::GetHoveredEdge() const
{
    return hoveredEdge;
}


void Map::SetHoveredEdge(int index)
{
    if (index < -1 ||
        index >= static_cast<int>(mapData.edges.size()))
    {
        hoveredEdge = -1;
        return;
    }

    hoveredEdge = index;
}


// ============================================================
// Undo
// ============================================================

void Map::SaveUndoState()
{
    undoStack.push_back(mapData);

    constexpr size_t MAX_UNDO_STATES = 64;

    if (undoStack.size() > MAX_UNDO_STATES)
    {
        undoStack.erase(
            undoStack.begin()
        );
    }
}


void Map::Undo()
{
    if (undoStack.empty())
        return;

    mapData = undoStack.back();

    undoStack.pop_back();

    selectedSurface = -1;
    hoveredSurface = -1;

    selectedVertex = -1;
    hoveredVertex = -1;

    selectedEdge = -1;
    hoveredEdge = -1;

    BuildRenderMeshes();
}


// ============================================================
// Vertex movement
// ============================================================

void Map::MoveSelectedVertex(
    const glm::vec3& delta
)
{
    if (selectedVertex < 0 ||
        selectedVertex >=
            static_cast<int>(mapData.vertices.size()))
    {
        return;
    }

    SaveUndoState();

    mapData.vertices[selectedVertex].position += delta;

    BuildRenderMeshes();
}


void Map::SetSelectedVertexPosition(
    const glm::vec3& position
)
{
    if (selectedVertex < 0 ||
        selectedVertex >=
            static_cast<int>(mapData.vertices.size()))
    {
        return;
    }

    SaveUndoState();

    mapData.vertices[selectedVertex].position =
        position;

    BuildRenderMeshes();
}


glm::vec3 Map::GetSelectedVertexPosition() const
{
    if (selectedVertex < 0 ||
        selectedVertex >=
            static_cast<int>(mapData.vertices.size()))
    {
        return glm::vec3(0.0f);
    }

    return mapData.vertices[selectedVertex].position;
}


// ============================================================
// Surface movement
// ============================================================

void Map::MoveSelectedSurface(
    const glm::vec3& delta
)
{
    if (selectedSurface < 0 ||
        selectedSurface >=
            static_cast<int>(mapData.surfaces.size()))
    {
        return;
    }

    SaveUndoState();

    std::vector<unsigned int> vertices =
        GetSurfaceVertices(
            mapData.surfaces[selectedSurface],
            mapData
        );

    for (unsigned int vertexIndex : vertices)
    {
        if (vertexIndex >= mapData.vertices.size())
            continue;

        mapData.vertices[vertexIndex].position += delta;
    }

    BuildRenderMeshes();
}

void Map::MoveSelectedEdge(
    const glm::vec3& delta
)
{
    if (selectedEdge < 0 ||
        selectedEdge >=
            static_cast<int>(mapData.edges.size()))
    {
        return;
    }

    SaveUndoState();

    std::vector<unsigned int> vertices =
        GetEdgeVertices(
            mapData.edges[selectedEdge],
            mapData
        );

    for (unsigned int vertexIndex : vertices)
    {
        if (vertexIndex >= mapData.vertices.size())
            continue;

        mapData.vertices[vertexIndex].position += delta;
    }

    BuildRenderMeshes();
}

void Map::SetSelectedSurfacePosition(
    const glm::vec3& position
)
{
    if (selectedSurface < 0 ||
        selectedSurface >=
            static_cast<int>(mapData.surfaces.size()))
    {
        return;
    }

    std::vector<unsigned int> vertices =
        GetSurfaceVertices(
            mapData.surfaces[selectedSurface],
            mapData
        );

    if (vertices.empty())
        return;

    SaveUndoState();

    glm::vec3 currentCenter(0.0f);

    for (unsigned int vertexIndex : vertices)
    {
        if (vertexIndex >= mapData.vertices.size())
            continue;

        currentCenter +=
            mapData.vertices[vertexIndex].position;
    }

    currentCenter /=
        static_cast<float>(vertices.size());

    glm::vec3 delta =
        position - currentCenter;

    for (unsigned int vertexIndex : vertices)
    {
        if (vertexIndex >= mapData.vertices.size())
            continue;

        mapData.vertices[vertexIndex].position += delta;
    }

    BuildRenderMeshes();
}


glm::vec3 Map::GetSelectedSurfacePosition() const
{
    if (selectedSurface < 0 ||
        selectedSurface >=
            static_cast<int>(mapData.surfaces.size()))
    {
        return glm::vec3(0.0f);
    }

    std::vector<unsigned int> vertices =
        GetSurfaceVertices(
            mapData.surfaces[selectedSurface],
            mapData
        );

    if (vertices.empty())
        return glm::vec3(0.0f);

    glm::vec3 center(0.0f);

    int count = 0;

    for (unsigned int vertexIndex : vertices)
    {
        if (vertexIndex >= mapData.vertices.size())
            continue;

        center +=
            mapData.vertices[vertexIndex].position;

        count++;
    }

    if (count == 0)
        return glm::vec3(0.0f);

    return center /
        static_cast<float>(count);
}

// ============================================================
// Edge / Surface helpers
// ============================================================

std::vector<unsigned int>
Map::GetSurfaceEdges(
    const EditorSurface& surface
) const
{
    return surface.edges;
}


std::vector<unsigned int>
Map::GetEdgesUsingVertex(
    unsigned int vertex
) const
{
    std::vector<unsigned int> result;

    for (unsigned int i = 0;
         i < mapData.edges.size();
         i++)
    {
        const EditorEdge& edge =
            mapData.edges[i];

        if (edge.vertex0 == vertex ||
            edge.vertex1 == vertex)
        {
            result.push_back(i);
        }
    }

    return result;
}


std::vector<unsigned int>
Map::GetSurfacesUsingEdge(
    unsigned int edge
) const
{
    std::vector<unsigned int> result;

    for (unsigned int i = 0;
         i < mapData.surfaces.size();
         i++)
    {
        const EditorSurface& surface =
            mapData.surfaces[i];

        if (std::find(
                surface.edges.begin(),
                surface.edges.end(),
                edge
            ) != surface.edges.end())
        {
            result.push_back(i);
        }
    }

    return result;
}


std::vector<unsigned int>
Map::GetSurfacesUsingVertex(
    unsigned int vertex
) const
{
    std::vector<unsigned int> result;

    for (unsigned int i = 0;
         i < mapData.surfaces.size();
         i++)
    {
        std::vector<unsigned int> vertices =
            GetSurfaceVertices(
                mapData.surfaces[i]
            );

        if (std::find(
                vertices.begin(),
                vertices.end(),
                vertex
            ) != vertices.end())
        {
            result.push_back(i);
        }
    }

    return result;
}


int Map::FindEdge(
    unsigned int vertex0,
    unsigned int vertex1
) const
{
    for (unsigned int i = 0;
         i < mapData.edges.size();
         i++)
    {
        const EditorEdge& edge =
            mapData.edges[i];

        if ((edge.vertex0 == vertex0 &&
             edge.vertex1 == vertex1) ||

            (edge.vertex0 == vertex1 &&
             edge.vertex1 == vertex0))
        {
            return static_cast<int>(i);
        }
    }

    return -1;
}


bool Map::IsVertexUsed(
    unsigned int vertexIndex
) const
{
    for (const EditorEdge& edge :
         mapData.edges)
    {
        if (edge.vertex0 == vertexIndex ||
            edge.vertex1 == vertexIndex)
        {
            return true;
        }
    }

    return false;
}

std::vector<unsigned int>
Map::GetSurfaceVertices(
    const EditorSurface& surface
) const
{
    return GetSurfaceVertices(
        surface,
        mapData
    );
}

// ============================================================
// Draw Edges
// ============================================================

void Map::DrawEdges()
{
    for (unsigned int i = 0;
         i < mapData.edges.size();
         i++)
    {
        const EditorEdge& edge =
            mapData.edges[i];

        if (edge.vertex0 >= mapData.vertices.size() ||
            edge.vertex1 >= mapData.vertices.size())
        {
            continue;
        }

        const glm::vec3& a =
            mapData.vertices[edge.vertex0].position;

        const glm::vec3& b =
            mapData.vertices[edge.vertex1].position;

        Vector3 start =
        {
            a.x,
            a.y,
            a.z
        };

        Vector3 end =
        {
            b.x,
            b.y,
            b.z
        };

        Color color = GRAY;

        if (static_cast<int>(i) == hoveredEdge)
            color = YELLOW;

        if (static_cast<int>(i) == selectedEdge)
            color = RED;

        DrawLine3D(
            start,
            end,
            color
        );
    }
}


// ============================================================
// Draw Selection
// ============================================================

void Map::DrawSelection()
{
    // --------------------------------------------------------
    // Hovered surface
    // --------------------------------------------------------

    if (hoveredSurface >= 0 &&
        hoveredSurface <
            static_cast<int>(mapData.surfaces.size()))
    {
        const EditorSurface& surface =
            mapData.surfaces[hoveredSurface];

        std::vector<unsigned int> vertices =
            GetSurfaceVertices(surface);

        if (vertices.size() >= 3)
        {
            for (size_t i = 1;
                 i + 1 < vertices.size();
                 i++)
            {
                if (vertices[0] >= mapData.vertices.size() ||
                    vertices[i] >= mapData.vertices.size() ||
                    vertices[i + 1] >= mapData.vertices.size())
                {
                    continue;
                }

                const glm::vec3& a =
                    mapData.vertices[vertices[0]].position;

                const glm::vec3& b =
                    mapData.vertices[vertices[i]].position;

                const glm::vec3& c =
                    mapData.vertices[vertices[i + 1]].position;

                DrawTriangle3D(
                    {
                        a.x,
                        a.y,
                        a.z
                    },
                    {
                        b.x,
                        b.y,
                        b.z
                    },
                    {
                        c.x,
                        c.y,
                        c.z
                    },
                    YELLOW
                );
            }
        }
    }


    // --------------------------------------------------------
    // Selected surface edges
    // --------------------------------------------------------

    if (selectedSurface >= 0 &&
        selectedSurface <
            static_cast<int>(mapData.surfaces.size()))
    {
        const EditorSurface& surface =
            mapData.surfaces[selectedSurface];

        std::vector<unsigned int> vertices =
            GetSurfaceVertices(surface);

        if (vertices.size() >= 2)
        {
            for (size_t i = 0;
                 i < vertices.size();
                 i++)
            {
                unsigned int aIndex =
                    vertices[i];

                unsigned int bIndex =
                    vertices[
                        (i + 1) % vertices.size()
                    ];

                if (aIndex >= mapData.vertices.size() ||
                    bIndex >= mapData.vertices.size())
                {
                    continue;
                }

                const glm::vec3& a =
                    mapData.vertices[aIndex].position;

                const glm::vec3& b =
                    mapData.vertices[bIndex].position;

                DrawThickFatyLine3D(
                    {
                        a.x,
                        a.y,
                        a.z
                    },
                    {
                        b.x,
                        b.y,
                        b.z
                    },
                    lineWidth,
                    RED
                );
            }
        }
    }
}

std::vector<unsigned int>
Map::GetEdgeVertices(
    const EditorEdge& edge,
    const EditorData& data
) const
{
    std::vector<unsigned int> vertices;

    if (edge.vertex0 < data.vertices.size())
        vertices.push_back(edge.vertex0);

    if (edge.vertex1 < data.vertices.size() &&
        edge.vertex1 != edge.vertex0)
    {
        vertices.push_back(edge.vertex1);
    }

    return vertices;
}

std::vector<unsigned int> Map::GetEdgeVertices(
    const EditorEdge& edge
) const
{
    return GetEdgeVertices(
        edge,
        mapData
    );
}

void Map::DrawEdgeSelection()
{

    if (hoveredEdge >= 0 &&
        hoveredEdge <
            static_cast<int>(mapData.edges.size()))
    {
        const EditorEdge& edge =
            mapData.edges[hoveredEdge];

        std::vector<unsigned int> vertices =
            GetEdgeVertices(edge);

        if (vertices.size() >= 3)
        {
            for (size_t i = 1;
                 i + 1 < vertices.size();
                 i++)
            {
                if (vertices[0] >= mapData.vertices.size() ||
                    vertices[i] >= mapData.vertices.size() ||
                    vertices[i + 1] >= mapData.vertices.size())
                {
                    continue;
                }

                const glm::vec3& a =
                    mapData.vertices[vertices[0]].position;

                const glm::vec3& b =
                    mapData.vertices[vertices[i]].position;

                const glm::vec3& c =
                    mapData.vertices[vertices[i + 1]].position;

                DrawTriangle3D(
                    {
                        a.x,
                        a.y,
                        a.z
                    },
                    {
                        b.x,
                        b.y,
                        b.z
                    },
                    {
                        c.x,
                        c.y,
                        c.z
                    },
                    YELLOW
                );
            }
        }
    }


    // --------------------------------------------------------
    // Selected surface edges
    // --------------------------------------------------------

    if (selectedSurface >= 0 &&
        selectedSurface <
            static_cast<int>(mapData.surfaces.size()))
    {
        const EditorSurface& surface =
            mapData.surfaces[selectedSurface];

        std::vector<unsigned int> vertices =
            GetSurfaceVertices(surface);

        if (vertices.size() >= 2)
        {
            for (size_t i = 0;
                 i < vertices.size();
                 i++)
            {
                unsigned int aIndex =
                    vertices[i];

                unsigned int bIndex =
                    vertices[
                        (i + 1) % vertices.size()
                    ];

                if (aIndex >= mapData.vertices.size() ||
                    bIndex >= mapData.vertices.size())
                {
                    continue;
                }

                const glm::vec3& a =
                    mapData.vertices[aIndex].position;

                const glm::vec3& b =
                    mapData.vertices[bIndex].position;

                DrawThickFatyLine3D(
                    {
                        a.x,
                        a.y,
                        a.z
                    },
                    {
                        b.x,
                        b.y,
                        b.z
                    },
                    lineWidth,
                    RED
                );
            }
        }
    }
}

// ============================================================
// Draw Surface Vertex Selection
// ============================================================

void Map::DrawSurfaceVertexSelection()
{
    for (int i = 0;
         i < static_cast<int>(mapData.vertices.size());
         i++)
    {
        const glm::vec3& position =
            mapData.vertices[i].position;

        Color color = BLANK;

        if (i == hoveredVertex)
            color = YELLOW;

        if (i == selectedVertex)
            color = RED;

        DrawSphere(
            {
                position.x,
                position.y,
                position.z
            },
            vertexSelectionWidth,
            color
        );
    }
}


// ============================================================
// Draw Orphaned Vertices
// ============================================================

void Map::DrawOrphanedVertices()
{
    for (unsigned int i = 0;
         i < mapData.vertices.size();
         i++)
    {
        if (IsVertexUsed(i))
            continue;

        const glm::vec3& position =
            mapData.vertices[i].position;

        DrawSphere(
            {
                position.x,
                position.y,
                position.z
            },
            vertexSelectionWidth,
            PURPLE
        );
    }
}


// ============================================================
// Pick Surface
// ============================================================

int Map::PickSurface(
    const Ray& ray
)
{
    float closestDistance = FLT_MAX;

    int closestSurface = -1;

    for (int surfaceIndex = 0;
         surfaceIndex <
             static_cast<int>(mapData.surfaces.size());
         surfaceIndex++)
    {
        const EditorSurface& surface =
            mapData.surfaces[surfaceIndex];

        std::vector<unsigned int> vertices =
            GetSurfaceVertices(surface);

        if (vertices.size() < 3)
            continue;

        if (vertices[0] >= mapData.vertices.size())
            continue;

        Vector3 a =
        {
            mapData.vertices[
                vertices[0]
            ].position.x,

            mapData.vertices[
                vertices[0]
            ].position.y,

            mapData.vertices[
                vertices[0]
            ].position.z
        };

        for (size_t i = 1;
             i + 1 < vertices.size();
             i++)
        {
            if (vertices[i] >= mapData.vertices.size() ||
                vertices[i + 1] >= mapData.vertices.size())
            {
                continue;
            }

            Vector3 b =
            {
                mapData.vertices[
                    vertices[i]
                ].position.x,

                mapData.vertices[
                    vertices[i]
                ].position.y,

                mapData.vertices[
                    vertices[i]
                ].position.z
            };

            Vector3 c =
            {
                mapData.vertices[
                    vertices[i + 1]
                ].position.x,

                mapData.vertices[
                    vertices[i + 1]
                ].position.y,

                mapData.vertices[
                    vertices[i + 1]
                ].position.z
            };

            RayCollision collision =
                GetRayCollisionTriangle(
                    ray,
                    a,
                    b,
                    c
                );

            if (collision.hit &&
                collision.distance < closestDistance)
            {
                closestDistance =
                    collision.distance;

                closestSurface =
                    surfaceIndex;
            }
        }
    }

    return closestSurface;
}


int Map::PickEdge(
    const Ray& ray
)
{
    float closestDistance = FLT_MAX;

    int closestEdge = -1;

    for (int edgeIndex = 0;
         edgeIndex <
             static_cast<int>(mapData.edges.size());
         edgeIndex++)
    {
        const EditorEdge& edge =
            mapData.edges[edgeIndex];

        std::vector<unsigned int> vertices =
            GetEdgeVertices(edge);

        if (vertices.size() < 3)
            continue;

        if (vertices[0] >= mapData.edges.size())
            continue;

        Vector3 a =
        {
            mapData.vertices[
                vertices[0]
            ].position.x,

            mapData.vertices[
                vertices[0]
            ].position.y,

            mapData.vertices[
                vertices[0]
            ].position.z
        };

        for (size_t i = 1;
             i + 1 < vertices.size();
             i++)
        {
            if (vertices[i] >= mapData.vertices.size() ||
                vertices[i + 1] >= mapData.vertices.size())
            {
                continue;
            }

            Vector3 b =
            {
                mapData.vertices[
                    vertices[i]
                ].position.x,

                mapData.vertices[
                    vertices[i]
                ].position.y,

                mapData.vertices[
                    vertices[i]
                ].position.z
            };

            Vector3 c =
            {
                mapData.vertices[
                    vertices[i + 1]
                ].position.x,

                mapData.vertices[
                    vertices[i + 1]
                ].position.y,

                mapData.vertices[
                    vertices[i + 1]
                ].position.z
            };

            RayCollision collision =
                GetRayCollisionTriangle(
                    ray,
                    a,
                    b,
                    c
                );

            if (collision.hit &&
                collision.distance < closestDistance)
            {
                closestDistance =
                    collision.distance;

                closestEdge =
                    edgeIndex;
            }
        }
    }

    return closestEdge;
}


// ============================================================
// Pick Vertex
// ============================================================

int Map::PickVertex(
    const Ray& ray
)
{
    float closestDistance = FLT_MAX;

    int closestVertex = -1;

    constexpr float PICK_RADIUS = 0.25f;

    for (int i = 0;
         i < static_cast<int>(mapData.vertices.size());
         i++)
    {
        const glm::vec3& p =
            mapData.vertices[i].position;

        Vector3 position =
        {
            p.x,
            p.y,
            p.z
        };

        Vector3 toVertex =
            Vector3Subtract(
                position,
                ray.position
            );

        float distanceAlongRay =
            Vector3DotProduct(
                toVertex,
                ray.direction
            );

        if (distanceAlongRay < 0.0f)
            continue;

        Vector3 closestPoint =
            Vector3Add(
                ray.position,
                Vector3Scale(
                    ray.direction,
                    distanceAlongRay
                )
            );

        float distance =
            Vector3Distance(
                closestPoint,
                position
            );

        if (distance <= PICK_RADIUS &&
            distanceAlongRay < closestDistance)
        {
            closestDistance =
                distanceAlongRay;

            closestVertex =
                i;
        }
    }

    return closestVertex;
}


// ============================================================
// Delete Selected Vertex
// ============================================================

void Map::DeleteSelectedVertex()
{
    if (selectedVertex < 0 ||
        selectedVertex >=
            static_cast<int>(mapData.vertices.size()))
    {
        return;
    }

    SaveUndoState();

    unsigned int deletedVertex =
        static_cast<unsigned int>(
            selectedVertex
        );


    // --------------------------------------------------------
    // Remove surfaces using this vertex
    // --------------------------------------------------------

    for (auto it = mapData.surfaces.begin();
         it != mapData.surfaces.end();)
    {
        std::vector<unsigned int> vertices =
            GetSurfaceVertices(*it);

        bool usesVertex =
            std::find(
                vertices.begin(),
                vertices.end(),
                deletedVertex
            ) != vertices.end();

        if (usesVertex)
        {
            it =
                mapData.surfaces.erase(it);
        }
        else
        {
            ++it;
        }
    }


    // --------------------------------------------------------
    // Remove edges using this vertex
    // --------------------------------------------------------

    for (auto it = mapData.edges.begin();
         it != mapData.edges.end();)
    {
        if (it->vertex0 == deletedVertex ||
            it->vertex1 == deletedVertex)
        {
            it =
                mapData.edges.erase(it);
        }
        else
        {
            ++it;
        }
    }


    // --------------------------------------------------------
    // Remove vertex
    // --------------------------------------------------------

    mapData.vertices.erase(
        mapData.vertices.begin() +
        selectedVertex
    );


    // --------------------------------------------------------
    // Fix vertex indices
    // --------------------------------------------------------

    for (EditorEdge& edge :
         mapData.edges)
    {
        if (edge.vertex0 > deletedVertex)
            edge.vertex0--;

        if (edge.vertex1 > deletedVertex)
            edge.vertex1--;
    }


    selectedVertex = -1;
    hoveredVertex = -1;

    selectedSurface = -1;
    hoveredSurface = -1;

    selectedEdge = -1;
    hoveredEdge = -1;

    BuildRenderMeshes();
}


// ============================================================
// Delete Selected Surface
// ============================================================

void Map::DeleteSelectedSurface()
{
    if (selectedSurface < 0 ||
        selectedSurface >=
            static_cast<int>(mapData.surfaces.size()))
    {
        return;
    }

    SaveUndoState();

    mapData.surfaces.erase(
        mapData.surfaces.begin() +
        selectedSurface
    );

    selectedSurface = -1;
    hoveredSurface = -1;

    BuildRenderMeshes();
}

void Map::DrawThickFatyLine3D(const Vector3& start, const Vector3& end, float thickness, Color color)
{
    Vector3 direction =
        Vector3Subtract(end, start);

    float length =
        Vector3Length(direction);

    if (length <= 0.0001f)
        return;

    Vector3 directionNormalized =
        Vector3Normalize(direction);

    DrawCylinderEx(
        start,
        end,
        thickness,
        thickness,
        8,
        color
    );
}

void Map::RotateSelectedSurface(const glm::vec3& delta)
{
    if (selectedSurface < 0 ||
        selectedSurface >=
            static_cast<int>(mapData.surfaces.size()))
    {
        return;
    }

    SaveUndoState();

    EditorSurface& surface =
        mapData.surfaces[selectedSurface];

    std::vector<unsigned int> vertices =
        GetSurfaceVertices(surface, mapData);

    if (vertices.empty())
        return;

    glm::vec3 center(0.0f);

    int validVertexCount = 0;

    for (unsigned int vertexIndex : vertices)
    {
        if (vertexIndex >= mapData.vertices.size())
            continue;

        center += mapData.vertices[vertexIndex].position;
        validVertexCount++;
    }

    if (validVertexCount == 0)
        return;

    center /= static_cast<float>(validVertexCount);

    glm::mat4 rotationMatrix(1.0f);

    rotationMatrix = glm::rotate(
        rotationMatrix,
        glm::radians(delta.x),
        glm::vec3(1.0f, 0.0f, 0.0f)
    );

    rotationMatrix = glm::rotate(
        rotationMatrix,
        glm::radians(delta.y),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    rotationMatrix = glm::rotate(
        rotationMatrix,
        glm::radians(delta.z),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    for (unsigned int vertexIndex : vertices)
    {
        if (vertexIndex >= mapData.vertices.size())
            continue;

        glm::vec3 relativePosition =
            mapData.vertices[vertexIndex].position - center;

        glm::vec4 rotatedPosition =
            rotationMatrix *
            glm::vec4(relativePosition, 1.0f);

        mapData.vertices[vertexIndex].position =
            center + glm::vec3(rotatedPosition);
    }

    // Store the rotation
    surface.rotation += delta;

    BuildRenderMeshes();
}

void Map::ScaleSelectedSurface(const glm::vec3& scale)
{
    if (selectedSurface < 0 ||
        selectedSurface >=
            static_cast<int>(mapData.surfaces.size()))
    {
        return;
    }

    EditorSurface& surface =
        mapData.surfaces[selectedSurface];

    std::vector<unsigned int> vertices =
        GetSurfaceVertices(
            surface,
            mapData
        );

    if (vertices.empty())
        return;

    // --------------------------------------------------------
    // Calculate surface center
    // --------------------------------------------------------

    glm::vec3 center(0.0f);

    int validVertexCount = 0;

    for (unsigned int vertexIndex : vertices)
    {
        if (vertexIndex >= mapData.vertices.size())
            continue;

        center += mapData.vertices[vertexIndex].position;
        validVertexCount++;
    }

    if (validVertexCount == 0)
        return;

    center /= static_cast<float>(validVertexCount);

    // --------------------------------------------------------
    // Scale vertices around center
    // --------------------------------------------------------

    for (unsigned int vertexIndex : vertices)
    {
        if (vertexIndex >= mapData.vertices.size())
            continue;

        glm::vec3 relativePosition =
            mapData.vertices[vertexIndex].position - center;

        relativePosition.x *= scale.x;
        relativePosition.y *= scale.y;
        relativePosition.z *= scale.z;

        mapData.vertices[vertexIndex].position =
            center + relativePosition;
    }

    // --------------------------------------------------------
    // Store scale
    // --------------------------------------------------------

    surface.scale.x *= scale.x;
    surface.scale.y *= scale.y;
    surface.scale.z *= scale.z;

    BuildRenderMeshes();
}

void Map::DrawEntities(Camera camera)
{
    Texture2D playerSpawnSprite =
        LoadTexture(
            "Editor/Sprites/playerSpawn.png"
        );

    for (int i = 0;
         i < static_cast<int>(mapData.entities.size());
         i++)
    {
        const EditorEntity& entity =
            mapData.entities[i];

        if (entity.type == 1)
        {
            Texture2D entitySprite =
                LoadTexture(
                    "Editor/Sprites/playerSpawn.png"
                );

            DrawBillboard(
                camera,
                entitySprite,
                glmv3rayvec3(entity.position),
                1.0f,
                WHITE
            );
        }
    }

    UnloadTexture(playerSpawnSprite);
}

EditorEntity Map::GetEntity(unsigned int type)
{
    EditorEntity entity{};

    switch (type)
    {
        case 0:
            entity.type = 0;
            entity.position = glm::vec3(0.0f);
            entity.rotation = glm::vec3(0.0f);
            entity.scale = glm::vec3(1.0f);
            break;

        case 1:
            entity.type = 1;
            entity.position = glm::vec3(0.0f);
            entity.rotation = glm::vec3(0.0f);
            entity.scale = glm::vec3(1.0f);
            break;

        default:
            entity.type = 1;
            entity.position = glm::vec3(0.0f);
            entity.rotation = glm::vec3(0.0f);
            entity.scale = glm::vec3(1.0f);
            break;
    }

    return entity;
}

Vector3 Map::glmv3rayvec3(glm::vec3 value)
{
    Vector3 returnValue;

    returnValue.x = value.x;
    returnValue.y = value.y;
    returnValue.z = value.z;

    return returnValue;
}

int Map::PickEntity(
    const Ray& ray
)
{
    float closestDistance = FLT_MAX;

    int closestEntity = -1;

    constexpr float PICK_RADIUS = 0.25f;

    for (int i = 0;
         i < static_cast<int>(mapData.entities.size());
         i++)
    {
        const glm::vec3& p =
            mapData.entities[i].position;

        Vector3 position =
        {
            p.x,
            p.y,
            p.z
        };

        Vector3 toVertex =
            Vector3Subtract(
                position,
                ray.position
            );

        float distanceAlongRay =
            Vector3DotProduct(
                toVertex,
                ray.direction
            );

        if (distanceAlongRay < 0.0f)
            continue;

        Vector3 closestPoint =
            Vector3Add(
                ray.position,
                Vector3Scale(
                    ray.direction,
                    distanceAlongRay
                )
            );

        float distance =
            Vector3Distance(
                closestPoint,
                position
            );

        if (distance <= PICK_RADIUS &&
            distanceAlongRay < closestDistance)
        {
            closestDistance =
                distanceAlongRay;

            closestEntity =
                i;
        }
    }

    return closestEntity;
}

int Map::PickLight(
    const Ray& ray
)
{
    float closestDistance = FLT_MAX;

    int closestLight = -1;

    constexpr float PICK_RADIUS = 0.25f;

    for (int i = 0;
         i < static_cast<int>(mapData.lights.size());
         i++)
    {
        const glm::vec3& p =
            mapData.lights[i].position;

        Vector3 position =
        {
            p.x,
            p.y,
            p.z
        };

        Vector3 toVertex =
            Vector3Subtract(
                position,
                ray.position
            );

        float distanceAlongRay =
            Vector3DotProduct(
                toVertex,
                ray.direction
            );

        if (distanceAlongRay < 0.0f)
            continue;

        Vector3 closestPoint =
            Vector3Add(
                ray.position,
                Vector3Scale(
                    ray.direction,
                    distanceAlongRay
                )
            );

        float distance =
            Vector3Distance(
                closestPoint,
                position
            );

        if (distance <= PICK_RADIUS &&
            distanceAlongRay < closestDistance)
        {
            closestDistance =
                distanceAlongRay;

            closestLight =
                i;
        }
    }

    return closestLight;
}

void Map::AddLight(EditorLightType type)
{
    SaveUndoState();

    EditorLight light;

    light.type = type;

    light.position =
        glm::vec3(0.0f, 2.0f, 0.0f);

    light.direction =
        glm::vec3(0.0f, -1.0f, 0.0f);

    light.ambient =
        glm::vec3(0.2f);

    light.diffuse =
        glm::vec3(1.0f);

    light.constant = 1.0f;
    light.linear = 0.7f;
    light.quadratic = 3.0f;

    light.innerCone = 15.0f;
    light.outerCone = 25.0f;

    mapData.lights.push_back(light);

    selectedLight =
        static_cast<int>(
            mapData.lights.size() - 1
        );
}

void Map::DrawLights(Camera camera)
{
    Texture2D lightTex = LoadTexture("Editor/Sprites/light.png");

    for (int i = 0;
         i < static_cast<int>(mapData.lights.size());
         i++)
    {
        const EditorLight& light =
            mapData.lights[i];

        Vector3 position =
        {
            light.position.x,
            light.position.y,
            light.position.z
        };

        Color color = WHITE;

        if (i == selectedLight)
            color = RED;

        if (i == hoveredLight)
            color = GREEN;

        DrawBillboard(camera, lightTex, position, 1.0, color);

        // Spotlight direction
        if (light.type == EditorLightType::Spot)
        {
            Vector3 direction =
            {
                light.direction.x,
                light.direction.y,
                light.direction.z
            };

            direction =
                Vector3Normalize(direction);

            Vector3 end =
                Vector3Add(
                    position,
                    Vector3Scale(
                        direction,
                        1.0f
                    )
                );

            DrawLine3D(
                position,
                end,
                color
            );
        }
    }

    UnloadTexture(lightTex);
}

void Map::MoveSelectedLight(const glm::vec3& delta)
{
    if (selectedLight < 0 ||
        selectedLight >=
            static_cast<int>(mapData.lights.size()))
    {
        return;
    }

    SaveUndoState();

    mapData.lights[selectedLight].position += delta;

    BuildRenderMeshes();
}

unsigned int Map::AddVertex(
    const glm::vec3& position
)
{
    EditorVertex vertex;

    vertex.position = position;
    vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    vertex.color = glm::vec3(1.0f);
    vertex.uv = glm::vec2(0.0f);

    mapData.vertices.push_back(vertex);

    return static_cast<unsigned int>(
        mapData.vertices.size() - 1
    );
}

unsigned int Map::AddSurface(
    const std::vector<unsigned int>& vertexIndices,
    unsigned int material
)
{
    EditorSurface surface;

    surface.indices = vertexIndices;
    surface.material = material;

    mapData.surfaces.push_back(surface);

    return static_cast<unsigned int>(
        mapData.surfaces.size() - 1
    );
}