#include "Model.h"

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <algorithm>


std::vector<Model*> Model::modelCache;


// ============================================================
// Constructor
// ============================================================

Model::Model(
    const char* file,
    unsigned int instancing,
    std::vector<glm::mat4> instanceMatrix
)
{
    Model::file = file;

    // --------------------------------------------------------
    // Load JSON
    // --------------------------------------------------------

    std::string text =
        get_file_contents(file);

    if (text.empty())
    {
        throw std::runtime_error(
            std::string("Failed to read model: ") + file
        );
    }

    JSON =
        json::parse(text);


    // --------------------------------------------------------
    // Load binary buffer
    // --------------------------------------------------------

    data =
        getData();


    // --------------------------------------------------------
    // Instancing
    // --------------------------------------------------------

    Model::instancing =
        instancing;

    Model::instanceMatrix =
        instanceMatrix;


    // --------------------------------------------------------
    // Load default scene
    // --------------------------------------------------------

    if (
        JSON.contains("scenes") &&
        JSON.contains("scene")
    )
    {
        unsigned int sceneIndex =
            JSON["scene"];

        if (
            sceneIndex >=
            JSON["scenes"].size()
        )
        {
            throw std::runtime_error(
                "Invalid glTF scene index"
            );
        }

        auto& scene =
            JSON["scenes"][sceneIndex];


        if (scene.contains("nodes"))
        {
            for (auto& node : scene["nodes"])
            {
                traverseNode(
                    node
                );
            }
        }
    }

    // --------------------------------------------------------
    // Fallback
    // --------------------------------------------------------

    else if (JSON.contains("nodes"))
    {
        for (
            unsigned int i = 0;
            i < JSON["nodes"].size();
            i++
        )
        {
            traverseNode(i);
        }
    }
}


// ============================================================
// Draw
// ============================================================

void Model::Draw(
    Shader& shader,
    Camera& camera,
    const glm::mat4& transform
)
{
    for (
        unsigned int i = 0;
        i < meshes.size();
        i++
    )
    {
        glm::mat4 finalMatrix =
            transform *
            matricesMeshes[i];


        meshes[i].Mesh::Draw(
            shader,
            camera,
            finalMatrix
        );
    }
}


// ============================================================
// Init
// ============================================================

bool Model::Init()
{
    modelCache.clear();

    return true;
}


// ============================================================
// Shutdown
// ============================================================

void Model::Shutdown()
{
    for (Model* model : modelCache)
    {
        delete model;
    }

    modelCache.clear();
}


// ============================================================
// Pre-cache model
// ============================================================

ModelHandle Model::Pre_CacheModel(
    const std::string& path
)
{
    Model* model =
        new Model(
            path.c_str()
        );

    modelCache.push_back(
        model
    );

    return static_cast<ModelHandle>(
        modelCache.size() - 1
    );
}


// ============================================================
// Draw cached model
// ============================================================

void Model::DrawModel(
    ModelHandle handle,
    Shader& shader,
    Camera& camera,
    const glm::mat4& transform
)
{
    if (
        handle >=
        modelCache.size()
    )
    {
        std::cerr
            << "Invalid ModelHandle: "
            << handle
            << std::endl;

        return;
    }


    modelCache[handle]->Draw(
        shader,
        camera,
        transform
    );
}


// ============================================================
// Load mesh
// ============================================================

void Model::loadMesh(
    unsigned int indMesh
)
{
    if (!JSON.contains("meshes"))
    {
        throw std::runtime_error(
            "glTF contains no meshes"
        );
    }


    if (
        indMesh >=
        JSON["meshes"].size()
    )
    {
        throw std::runtime_error(
            "Invalid mesh index"
        );
    }


    auto& meshJSON =
        JSON["meshes"][indMesh];


    if (!meshJSON.contains("primitives"))
        return;


    // --------------------------------------------------------
    // A glTF mesh can have multiple primitives.
    // --------------------------------------------------------

    for (
        unsigned int primitiveIndex = 0;
        primitiveIndex <
        meshJSON["primitives"].size();
        primitiveIndex++
    )
    {
        auto& primitive =
            meshJSON["primitives"]
                    [primitiveIndex];


        if (!primitive.contains("attributes"))
        {
            std::cerr
                << "Primitive has no attributes\n";

            continue;
        }


        auto& attributes =
            primitive["attributes"];


        // ----------------------------------------------------
        // POSITION
        // ----------------------------------------------------

        if (!attributes.contains("POSITION"))
        {
            std::cerr
                << "Primitive has no POSITION\n";

            continue;
        }


        unsigned int posAccInd =
            attributes["POSITION"];


        std::vector<float> posVec =
            getFloats(
                JSON["accessors"][posAccInd]
            );


        std::vector<glm::vec3> positions =
            groupFloatsVec3(
                posVec
            );


        // ----------------------------------------------------
        // NORMAL
        // ----------------------------------------------------

        std::vector<glm::vec3> normals;


        if (attributes.contains("NORMAL"))
        {
            unsigned int normalAccInd =
                attributes["NORMAL"];


            std::vector<float> normalVec =
                getFloats(
                    JSON["accessors"][normalAccInd]
                );


            normals =
                groupFloatsVec3(
                    normalVec
                );
        }


        if (
            normals.size() !=
            positions.size()
        )
        {
            normals.resize(
                positions.size(),
                glm::vec3(
                    0.0f,
                    1.0f,
                    0.0f
                )
            );
        }


        // ----------------------------------------------------
        // TEXCOORD_0
        // ----------------------------------------------------

        std::vector<glm::vec2> texUVs;


        if (
            attributes.contains(
                "TEXCOORD_0"
            )
        )
        {
            unsigned int texAccInd =
                attributes["TEXCOORD_0"];


            std::vector<float> texVec =
                getFloats(
                    JSON["accessors"][texAccInd]
                );


            texUVs =
                groupFloatsVec2(
                    texVec
                );
        }


        if (
            texUVs.size() !=
            positions.size()
        )
        {
            texUVs.resize(
                positions.size(),
                glm::vec2(0.0f)
            );
        }


        // ----------------------------------------------------
        // JOINTS_0
        // ----------------------------------------------------

        std::vector<glm::ivec4> boneIDs;


        if (
            attributes.contains(
                "JOINTS_0"
            )
        )
        {
            unsigned int jointAccInd =
                attributes["JOINTS_0"];


            boneIDs =
                getJointIndices(
                    JSON["accessors"][jointAccInd]
                );
        }


        if (
            boneIDs.size() !=
            positions.size()
        )
        {
            boneIDs.resize(
                positions.size(),
                glm::ivec4(0)
            );
        }


        // ----------------------------------------------------
        // WEIGHTS_0
        // ----------------------------------------------------

        std::vector<glm::vec4> weights;


        if (
            attributes.contains(
                "WEIGHTS_0"
            )
        )
        {
            unsigned int weightAccInd =
                attributes["WEIGHTS_0"];


            std::vector<float> weightVec =
                getFloats(
                    JSON["accessors"][weightAccInd]
                );


            weights =
                groupFloatsVec4(
                    weightVec
                );
        }


        if (
            weights.size() !=
            positions.size()
        )
        {
            weights.resize(
                positions.size(),
                glm::vec4(0.0f)
            );
        }


        // ----------------------------------------------------
        // Indices
        // ----------------------------------------------------

        std::vector<GLuint> indices;


        if (
            primitive.contains(
                "indices"
            )
        )
        {
            unsigned int indAccInd =
                primitive["indices"];


            indices =
                getIndices(
                    JSON["accessors"][indAccInd]
                );
        }
        else
        {
            indices.resize(
                positions.size()
            );


            for (
                unsigned int i = 0;
                i < positions.size();
                i++
            )
            {
                indices[i] =
                    i;
            }
        }


        // ----------------------------------------------------
        // Textures
        // ----------------------------------------------------
        
        std::vector<Texture> textures =
            getTextures(
                primitive
            );

        std::cout
            << "Textures: "
            << textures.size()
            << '\n';

        // ----------------------------------------------------
        // Build vertices
        // ----------------------------------------------------

        std::vector<Vertex> vertices =
            assembleVertices(
                positions,
                normals,
                texUVs,
                boneIDs,
                weights
            );


        // ----------------------------------------------------
        // Create Mesh
        // ----------------------------------------------------

        meshes.push_back(
            Mesh(
                vertices,
                indices,
                textures,
                instancing,
                instanceMatrix
            )
        );
    }
}


// ============================================================
// Traverse node
// ============================================================

void Model::traverseNode(
    unsigned int nextNode,
    glm::mat4 matrix
)
{
    if (
        nextNode >=
        JSON["nodes"].size()
    )
    {
        std::cerr
            << "Invalid node index: "
            << nextNode
            << '\n';

        return;
    }


    json node =
        JSON["nodes"][nextNode];


    // --------------------------------------------------------
    // Translation
    // --------------------------------------------------------

    glm::vec3 translation(
        0.0f
    );


    if (
        node.contains(
            "translation"
        )
    )
    {
        translation =
            glm::vec3(
                node["translation"][0],
                node["translation"][1],
                node["translation"][2]
            );
    }


    // --------------------------------------------------------
    // Rotation
    // --------------------------------------------------------

    glm::quat rotation(
        1.0f,
        0.0f,
        0.0f,
        0.0f
    );


    if (
        node.contains(
            "rotation"
        )
    )
    {
        rotation =
            glm::quat(
                node["rotation"][3],
                node["rotation"][0],
                node["rotation"][1],
                node["rotation"][2]
            );
    }


    // --------------------------------------------------------
    // Scale
    // --------------------------------------------------------

    glm::vec3 scale(
        1.0f
    );


    if (
        node.contains(
            "scale"
        )
    )
    {
        scale =
            glm::vec3(
                node["scale"][0],
                node["scale"][1],
                node["scale"][2]
            );
    }


    // --------------------------------------------------------
    // Matrix
    // --------------------------------------------------------

    glm::mat4 matNode(
        1.0f
    );


    if (
        node.contains(
            "matrix"
        )
    )
    {
        float matValues[16];


        for (
            unsigned int i = 0;
            i < 16;
            i++
        )
        {
            matValues[i] =
                node["matrix"][i];
        }


        matNode =
            glm::make_mat4(
                matValues
            );
    }


    // --------------------------------------------------------
    // Transform
    // --------------------------------------------------------

    glm::mat4 trans =
        glm::translate(
            glm::mat4(1.0f),
            translation
        );


    glm::mat4 rot =
        glm::mat4_cast(
            rotation
        );


    glm::mat4 sca =
        glm::scale(
            glm::mat4(1.0f),
            scale
        );


    glm::mat4 matNextNode =
        matrix *
        matNode *
        trans *
        rot *
        sca;


    // --------------------------------------------------------
    // Mesh
    // --------------------------------------------------------

    if (
        node.contains(
            "mesh"
        )
    )
    {
        translationsMeshes.push_back(
            translation
        );

        rotationsMeshes.push_back(
            rotation
        );

        scalesMeshes.push_back(
            scale
        );

        matricesMeshes.push_back(
            matNextNode
        );


        loadMesh(
            node["mesh"]
        );
    }


    // --------------------------------------------------------
    // Children
    // --------------------------------------------------------

    if (
        node.contains(
            "children"
        )
    )
    {
        for (
            unsigned int i = 0;
            i < node["children"].size();
            i++
        )
        {
            traverseNode(
                node["children"][i],
                matNextNode
            );
        }
    }
}


// ============================================================
// Load binary data
// ============================================================

std::vector<unsigned char> Model::getData()
{
    if (
        !JSON.contains(
            "buffers"
        ) ||
        JSON["buffers"].empty()
    )
    {
        throw std::runtime_error(
            "glTF contains no buffers"
        );
    }


    std::string uri =
        JSON["buffers"][0].value(
            "uri",
            ""
        );


    if (uri.empty())
    {
        throw std::runtime_error(
            "Buffer has no URI"
        );
    }


    std::filesystem::path modelPath(
        file
    );


    std::filesystem::path binPath =
        modelPath.parent_path() /
        uri;


    std::string path =
        binPath.lexically_normal().string();


    std::cout
        << "Loading binary: "
        << path
        << '\n';


    std::string bytesText =
        get_file_contents(
            path.c_str()
        );


    if (bytesText.empty())
    {
        throw std::runtime_error(
            "Failed to load binary buffer: " +
            path
        );
    }


    return std::vector<unsigned char>(
        bytesText.begin(),
        bytesText.end()
    );
}


// ============================================================
// Get FLOAT accessor
// ============================================================

std::vector<float> Model::getFloats(
    json accessor
)
{
    std::vector<float> floatVec;


    if (!accessor.contains("bufferView"))
    {
        throw std::runtime_error(
            "Accessor has no bufferView"
        );
    }


    unsigned int buffViewInd =
        accessor["bufferView"];


    unsigned int count =
        accessor["count"];


    unsigned int accByteOffset =
        accessor.value(
            "byteOffset",
            0
        );


    std::string type =
        accessor["type"];


    unsigned int componentType =
        accessor["componentType"];


    if (
        componentType != 5126
    )
    {
        throw std::runtime_error(
            "Expected FLOAT accessor"
        );
    }


    unsigned int numComponents = 0;


    if (type == "SCALAR")
        numComponents = 1;

    else if (type == "VEC2")
        numComponents = 2;

    else if (type == "VEC3")
        numComponents = 3;

    else if (type == "VEC4")
        numComponents = 4;

    else
        throw std::runtime_error(
            "Unsupported accessor type: " +
            type
        );


    if (
        buffViewInd >=
        JSON["bufferViews"].size()
    )
    {
        throw std::runtime_error(
            "Invalid bufferView index"
        );
    }


    auto& bufferView =
        JSON["bufferViews"][buffViewInd];


    unsigned int byteOffset =
        bufferView.value(
            "byteOffset",
            0
        );


    unsigned int elementSize =
        sizeof(float) *
        numComponents;


    unsigned int byteStride =
        bufferView.value(
            "byteStride",
            elementSize
        );


    unsigned int beginningOfData =
        byteOffset +
        accByteOffset;


    for (
        unsigned int element = 0;
        element < count;
        element++
    )
    {
        unsigned int elementOffset =
            beginningOfData +
            element * byteStride;


        for (
            unsigned int component = 0;
            component < numComponents;
            component++
        )
        {
            unsigned int offset =
                elementOffset +
                component *
                sizeof(float);


            if (
                offset +
                sizeof(float) >
                data.size()
            )
            {
                throw std::runtime_error(
                    "Accessor reads outside buffer"
                );
            }


            float value;


            std::memcpy(
                &value,
                &data[offset],
                sizeof(float)
            );


            floatVec.push_back(
                value
            );
        }
    }


    return floatVec;
}


// ============================================================
// Get JOINTS_0
// ============================================================

std::vector<glm::ivec4> Model::getJointIndices(
    json accessor
)
{
    std::vector<glm::ivec4> result;


    if (!accessor.contains("bufferView"))
    {
        throw std::runtime_error(
            "Joint accessor has no bufferView"
        );
    }


    unsigned int bufferViewIndex =
        accessor["bufferView"];


    unsigned int count =
        accessor["count"];


    unsigned int accessorOffset =
        accessor.value(
            "byteOffset",
            0
        );


    unsigned int componentType =
        accessor["componentType"];


    auto& bufferView =
        JSON["bufferViews"]
             [bufferViewIndex];


    unsigned int bufferOffset =
        bufferView.value(
            "byteOffset",
            0
        );


    // --------------------------------------------------------
    // JOINTS_0 is usually UNSIGNED_BYTE or UNSIGNED_SHORT.
    // --------------------------------------------------------

    unsigned int componentSize;


    if (
        componentType == 5121
    )
    {
        componentSize = 1;
    }
    else if (
        componentType == 5123
    )
    {
        componentSize = 2;
    }
    else
    {
        throw std::runtime_error(
            "Unsupported JOINTS_0 component type"
        );
    }


    unsigned int elementSize =
        componentSize * 4;


    unsigned int byteStride =
        bufferView.value(
            "byteStride",
            elementSize
        );


    unsigned int start =
        bufferOffset +
        accessorOffset;


    for (
        unsigned int i = 0;
        i < count;
        i++
    )
    {
        unsigned int offset =
            start +
            i * byteStride;


        glm::ivec4 joints(
            0
        );


        for (
            unsigned int j = 0;
            j < 4;
            j++
        )
        {
            unsigned int componentOffset =
                offset +
                j * componentSize;


            if (
                componentOffset +
                componentSize >
                data.size()
            )
            {
                throw std::runtime_error(
                    "JOINTS_0 reads outside buffer"
                );
            }


            if (
                componentType == 5121
            )
            {
                joints[j] =
                    data[
                        componentOffset
                    ];
            }
            else
            {
                unsigned short value;


                std::memcpy(
                    &value,
                    &data[
                        componentOffset
                    ],
                    sizeof(
                        unsigned short
                    )
                );


                joints[j] =
                    value;
            }
        }


        result.push_back(
            joints
        );
    }


    return result;
}


// ============================================================
// Get indices
// ============================================================

std::vector<GLuint> Model::getIndices(
    json accessor
)
{
    std::vector<GLuint> indices;


    if (!accessor.contains("bufferView"))
    {
        throw std::runtime_error(
            "Index accessor has no bufferView"
        );
    }


    unsigned int buffViewInd =
        accessor["bufferView"];


    unsigned int count =
        accessor["count"];


    unsigned int accByteOffset =
        accessor.value(
            "byteOffset",
            0
        );


    unsigned int componentType =
        accessor["componentType"];


    auto& bufferView =
        JSON["bufferViews"][buffViewInd];


    unsigned int byteOffset =
        bufferView.value(
            "byteOffset",
            0
        );


    unsigned int beginningOfData =
        byteOffset +
        accByteOffset;


    // --------------------------------------------------------
    // UNSIGNED BYTE
    // --------------------------------------------------------

    if (
        componentType == 5121
    )
    {
        for (
            unsigned int i = 0;
            i < count;
            i++
        )
        {
            unsigned int offset =
                beginningOfData +
                i;


            if (
                offset >= data.size()
            )
            {
                throw std::runtime_error(
                    "Index accessor out of bounds"
                );
            }


            indices.push_back(
                static_cast<GLuint>(
                    data[offset]
                )
            );
        }
    }


    // --------------------------------------------------------
    // UNSIGNED SHORT
    // --------------------------------------------------------

    else if (
        componentType == 5123
    )
    {
        for (
            unsigned int i = 0;
            i < count;
            i++
        )
        {
            unsigned int offset =
                beginningOfData +
                i * 2;


            if (
                offset + 2 >
                data.size()
            )
            {
                throw std::runtime_error(
                    "Index accessor out of bounds"
                );
            }


            unsigned short value;


            std::memcpy(
                &value,
                &data[offset],
                sizeof(
                    unsigned short
                )
            );


            indices.push_back(
                static_cast<GLuint>(
                    value
                )
            );
        }
    }


    // --------------------------------------------------------
    // UNSIGNED INT
    // --------------------------------------------------------

    else if (
        componentType == 5125
    )
    {
        for (
            unsigned int i = 0;
            i < count;
            i++
        )
        {
            unsigned int offset =
                beginningOfData +
                i * 4;


            if (
                offset + 4 >
                data.size()
            )
            {
                throw std::runtime_error(
                    "Index accessor out of bounds"
                );
            }


            unsigned int value;


            std::memcpy(
                &value,
                &data[offset],
                sizeof(
                    unsigned int
                )
            );


            indices.push_back(
                static_cast<GLuint>(
                    value
                )
            );
        }
    }

    else
    {
        throw std::runtime_error(
            "Unsupported index component type: " +
            std::to_string(
                componentType
            )
        );
    }


    return indices;
}


// ============================================================
// Get texture from glTF material
// ============================================================

std::vector<Texture> Model::getTextures(
    const json& primitive
)
{
    std::vector<Texture> textures;


    if (
        !primitive.contains(
            "material"
        )
    )
    {
        return textures;
    }


    unsigned int materialIndex =
        primitive["material"];


    if (
        !JSON.contains(
            "materials"
        ) ||
        materialIndex >=
        JSON["materials"].size()
    )
    {
        return textures;
    }


    auto& material =
        JSON["materials"]
             [materialIndex];


    if (
        !material.contains(
            "pbrMetallicRoughness"
        )
    )
    {
        return textures;
    }


    auto& pbr =
        material[
            "pbrMetallicRoughness"
        ];


    if (
        !pbr.contains(
            "baseColorTexture"
        )
    )
    {
        return textures;
    }


    unsigned int textureIndex =
        pbr[
            "baseColorTexture"
        ][
            "index"
        ];


    if (
        !JSON.contains(
            "textures"
        ) ||
        textureIndex >=
        JSON["textures"].size()
    )
    {
        return textures;
    }


    auto& textureJSON =
        JSON["textures"]
             [textureIndex];


    if (
        !textureJSON.contains(
            "source"
        )
    )
    {
        return textures;
    }


    unsigned int imageIndex =
        textureJSON["source"];


    if (
        !JSON.contains(
            "images"
        ) ||
        imageIndex >=
        JSON["images"].size()
    )
    {
        return textures;
    }


    auto& image =
        JSON["images"]
             [imageIndex];


    if (
        !image.contains(
            "uri"
        )
    )
    {
        return textures;
    }


    std::string texPath =
        image["uri"];


    // --------------------------------------------------------
    // Check texture cache
    // --------------------------------------------------------

    for (
        unsigned int i = 0;
        i < loadedTexName.size();
        i++
    )
    {
        if (
            loadedTexName[i] ==
            texPath
        )
        {
            textures.push_back(
                loadedTex[i]
            );

            return textures;
        }
    }


    // --------------------------------------------------------
    // Build path
    // --------------------------------------------------------

    std::filesystem::path modelPath(
        file
    );


    std::filesystem::path texturePath =
        modelPath.parent_path() /
        texPath;


    std::string fullPath =
        texturePath.lexically_normal().string();


    // --------------------------------------------------------
    // Load
    // --------------------------------------------------------

    std::cout
    << "Loading texture: "
    << fullPath
    << std::endl;

    Texture diffuse(
        fullPath.c_str(),
        "diffuse",
        loadedTex.size()
    );


    textures.push_back(
        diffuse
    );


    loadedTex.push_back(
        diffuse
    );


    loadedTexName.push_back(
        texPath
    );


    return textures;
}


// ============================================================
// Assemble vertices
// ============================================================

std::vector<Vertex> Model::assembleVertices(
    const std::vector<glm::vec3> positions,
    const std::vector<glm::vec3> normals,
    const std::vector<glm::vec2> texUVs,
    const std::vector<glm::ivec4> boneIDs,
    const std::vector<glm::vec4> weights
)
{
    std::vector<Vertex> vertices;


    vertices.reserve(
        positions.size()
    );


    for (
        size_t i = 0;
        i < positions.size();
        i++
    )
    {
        glm::vec3 normal =
            i < normals.size()
                ? normals[i]
                : glm::vec3(
                    0.0f,
                    1.0f,
                    0.0f
                );


        glm::vec2 uv =
            i < texUVs.size()
                ? texUVs[i]
                : glm::vec2(
                    0.0f
                );


        glm::ivec4 joints =
            i < boneIDs.size()
                ? boneIDs[i]
                : glm::ivec4(
                    0
                );


        glm::vec4 weight =
            i < weights.size()
                ? weights[i]
                : glm::vec4(
                    0.0f
                );


        vertices.push_back(
            Vertex
            {
                positions[i],
                normal,
                glm::vec3(
                    1.0f
                ),
                uv,
                joints,
                weight
            }
        );
    }


    return vertices;
}


// ============================================================
// Vec2
// ============================================================

std::vector<glm::vec2> Model::groupFloatsVec2(
    std::vector<float> floatVec
)
{
    std::vector<glm::vec2> vectors;


    for (
        unsigned int i = 0;
        i + 1 < floatVec.size();
        i += 2
    )
    {
        vectors.push_back(
            glm::vec2(
                floatVec[i],
                floatVec[i + 1]
            )
        );
    }


    return vectors;
}


// ============================================================
// Vec3
// ============================================================

std::vector<glm::vec3> Model::groupFloatsVec3(
    std::vector<float> floatVec
)
{
    std::vector<glm::vec3> vectors;


    for (
        unsigned int i = 0;
        i + 2 < floatVec.size();
        i += 3
    )
    {
        vectors.push_back(
            glm::vec3(
                floatVec[i],
                floatVec[i + 1],
                floatVec[i + 2]
            )
        );
    }


    return vectors;
}


// ============================================================
// Vec4
// ============================================================

std::vector<glm::vec4> Model::groupFloatsVec4(
    std::vector<float> floatVec
)
{
    std::vector<glm::vec4> vectors;


    for (
        unsigned int i = 0;
        i + 3 < floatVec.size();
        i += 4
    )
    {
        vectors.push_back(
            glm::vec4(
                floatVec[i],
                floatVec[i + 1],
                floatVec[i + 2],
                floatVec[i + 3]
            )
        );
    }


    return vectors;
}


// std::vector<Model*> Model::modelCache;

// // Model::Model(const char* file, unsigned int instancing, std::vector<glm::mat4> instanceMatrix)
// // {
// // 	// Make a JSON object
// // 	std::string text = get_file_contents(file);
// // 	JSON = json::parse(text);

// // 	// Get the binary data
// // 	Model::file = file;
// // 	data = getData();

// // 	Model::instancing = instancing;
// // 	Model::instanceMatrix = instanceMatrix;

// // 	// Traverse all nodes
// // 	traverseNode(0);
// // }

// Model::Model(
//     const char* file,
//     unsigned int instancing,
//     std::vector<glm::mat4> instanceMatrix
// )
// {
//     Model::file = file;

//     // --------------------------------------------------------
//     // Load JSON
//     // --------------------------------------------------------

//     std::string text =
//         get_file_contents(file);

//     if (text.empty())
//     {
//         throw std::runtime_error(
//             std::string("Failed to read model: ") + file
//         );
//     }

//     JSON =
//         json::parse(text);


//     // --------------------------------------------------------
//     // Load binary buffer
//     // --------------------------------------------------------

//     data =
//         getData();


//     // --------------------------------------------------------
//     // Instancing
//     // --------------------------------------------------------

//     Model::instancing =
//         instancing;

//     Model::instanceMatrix =
//         instanceMatrix;


//     // --------------------------------------------------------
//     // Load default scene
//     // --------------------------------------------------------

//     if (
//         JSON.contains("scenes") &&
//         JSON.contains("scene")
//     )
//     {
//         unsigned int sceneIndex =
//             JSON["scene"];

//         if (
//             sceneIndex >=
//             JSON["scenes"].size()
//         )
//         {
//             throw std::runtime_error(
//                 "Invalid glTF scene index"
//             );
//         }

//         auto& scene =
//             JSON["scenes"][sceneIndex];


//         if (scene.contains("nodes"))
//         {
//             for (auto& node : scene["nodes"])
//             {
//                 traverseNode(
//                     node
//                 );
//             }
//         }
//     }

//     // --------------------------------------------------------
//     // Fallback
//     // --------------------------------------------------------

//     else if (JSON.contains("nodes"))
//     {
//         for (
//             unsigned int i = 0;
//             i < JSON["nodes"].size();
//             i++
//         )
//         {
//             traverseNode(i);
//         }
//     }
// }

// // void Model::Draw(Shader& shader, Camera& camera, const glm::mat4& transform)
// // {
// // 	// Go over all meshes and draw each one
// // 	for (unsigned int i = 0; i < meshes.size(); i++)
// // 	{
// // 		glm::mat4 finalMatrix =
// //         	transform * matricesMeshes[i];

// // 		meshes[i].Mesh::Draw(shader, camera, finalMatrix);
// // 		// std::cout << "Meshes: " << meshes.size() << '\n';
// // 	}
// // }

// void Model::Draw(
//     Shader& shader,
//     Camera& camera,
//     const glm::mat4& transform
// )
// {
//     for (
//         unsigned int i = 0;
//         i < meshes.size();
//         i++
//     )
//     {
//         glm::mat4 finalMatrix =
//             transform *
//             matricesMeshes[i];


//         meshes[i].Mesh::Draw(
//             shader,
//             camera,
//             finalMatrix
//         );
//     }
// }

// bool Model::Init()
// {
//     modelCache.clear();

//     return true;
// }

// void Model::Shutdown()
// {
//     for (Model* model : modelCache)
//     {
//         delete model;
//     }

//     modelCache.clear();
// }

// // ModelHandle Model::Pre_CacheModel(const std::string& path)
// // {
// //     Model* model = new Model(path.c_str());

// //     modelCache.push_back(model);

// //     return static_cast<ModelHandle>(modelCache.size() - 1);
// // }

// ModelHandle Model::Pre_CacheModel(
//     const std::string& path
// )
// {
//     Model* model =
//         new Model(
//             path.c_str()
//         );

//     modelCache.push_back(
//         model
//     );

//     return static_cast<ModelHandle>(
//         modelCache.size() - 1
//     );
// }

// // void Model::DrawModel(
// //     ModelHandle handle,
// //     Shader& shader,
// //     Camera& camera,
// //     const glm::mat4& transform
// // )
// // {
// //     if (handle >= modelCache.size())
// //     {
// //         std::cerr << "Invalid ModelHandle: "
// //                   << handle
// //                   << std::endl;

// //         return;
// //     }

// //     modelCache[handle]->Draw(
// //         shader,
// //         camera,
// //         transform
// //     );
// // }

// void Model::DrawModel(
//     ModelHandle handle,
//     Shader& shader,
//     Camera& camera,
//     const glm::mat4& transform
// )
// {
//     if (
//         handle >=
//         modelCache.size()
//     )
//     {
//         std::cerr
//             << "Invalid ModelHandle: "
//             << handle
//             << std::endl;

//         return;
//     }


//     modelCache[handle]->Draw(
//         shader,
//         camera,
//         transform
//     );
// }

// // void Model::loadMesh(unsigned int indMesh)
// // {
// // 	// Get all accessor indices
// // 	unsigned int posAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
// // 	unsigned int normalAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
// // 	unsigned int texAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
// // 	unsigned int indAccInd = JSON["meshes"][indMesh]["primitives"][0]["indices"];

// // 	// Use accessor indices to get all vertices components
// // 	std::vector<float> posVec = getFloats(JSON["accessors"][posAccInd]);
// // 	std::vector<glm::vec3> positions = groupFloatsVec3(posVec);
// // 	std::vector<float> normalVec = getFloats(JSON["accessors"][normalAccInd]);
// // 	std::vector<glm::vec3> normals = groupFloatsVec3(normalVec);
// // 	std::vector<float> texVec = getFloats(JSON["accessors"][texAccInd]);
// // 	std::vector<glm::vec2> texUVs = groupFloatsVec2(texVec);

// // 	// Combine all the vertex components and also get the indices and textures
// // 	std::vector<Vertex> vertices = assembleVertices(positions, normals, texUVs);
// // 	std::vector<GLuint> indices = getIndices(JSON["accessors"][indAccInd]);
// // 	std::vector<Texture> textures = getTextures();

// // 	// std::cout << "Loading mesh " << indMesh << '\n';
// // 	// std::cout << "Vertices: " << positions.size() << '\n';
// // 	// std::cout << "Indices : " << indices.size() << '\n';

// // 	// Combine the vertices, indices, and textures into a mesh
// // 	meshes.push_back(Mesh(vertices, indices, textures, instancing, instanceMatrix));
// // }

// void Model::loadMesh(
//     unsigned int indMesh
// )
// {
//     if (!JSON.contains("meshes"))
//     {
//         throw std::runtime_error(
//             "glTF contains no meshes"
//         );
//     }


//     if (
//         indMesh >=
//         JSON["meshes"].size()
//     )
//     {
//         throw std::runtime_error(
//             "Invalid mesh index"
//         );
//     }


//     auto& meshJSON =
//         JSON["meshes"][indMesh];


//     if (!meshJSON.contains("primitives"))
//         return;


//     // --------------------------------------------------------
//     // A glTF mesh can have multiple primitives.
//     // --------------------------------------------------------

//     for (
//         unsigned int primitiveIndex = 0;
//         primitiveIndex <
//         meshJSON["primitives"].size();
//         primitiveIndex++
//     )
//     {
//         auto& primitive =
//             meshJSON["primitives"]
//                     [primitiveIndex];


//         if (!primitive.contains("attributes"))
//         {
//             std::cerr
//                 << "Primitive has no attributes\n";

//             continue;
//         }


//         auto& attributes =
//             primitive["attributes"];


//         // ----------------------------------------------------
//         // POSITION
//         // ----------------------------------------------------

//         if (!attributes.contains("POSITION"))
//         {
//             std::cerr
//                 << "Primitive has no POSITION\n";

//             continue;
//         }


//         unsigned int posAccInd =
//             attributes["POSITION"];


//         std::vector<float> posVec =
//             getFloats(
//                 JSON["accessors"][posAccInd]
//             );


//         std::vector<glm::vec3> positions =
//             groupFloatsVec3(
//                 posVec
//             );


//         // ----------------------------------------------------
//         // NORMAL
//         // ----------------------------------------------------

//         std::vector<glm::vec3> normals;


//         if (attributes.contains("NORMAL"))
//         {
//             unsigned int normalAccInd =
//                 attributes["NORMAL"];


//             std::vector<float> normalVec =
//                 getFloats(
//                     JSON["accessors"][normalAccInd]
//                 );


//             normals =
//                 groupFloatsVec3(
//                     normalVec
//                 );
//         }


//         if (
//             normals.size() !=
//             positions.size()
//         )
//         {
//             normals.resize(
//                 positions.size(),
//                 glm::vec3(
//                     0.0f,
//                     1.0f,
//                     0.0f
//                 )
//             );
//         }


//         // ----------------------------------------------------
//         // TEXCOORD_0
//         // ----------------------------------------------------

//         std::vector<glm::vec2> texUVs;


//         if (
//             attributes.contains(
//                 "TEXCOORD_0"
//             )
//         )
//         {
//             unsigned int texAccInd =
//                 attributes["TEXCOORD_0"];


//             std::vector<float> texVec =
//                 getFloats(
//                     JSON["accessors"][texAccInd]
//                 );


//             texUVs =
//                 groupFloatsVec2(
//                     texVec
//                 );
//         }


//         if (
//             texUVs.size() !=
//             positions.size()
//         )
//         {
//             texUVs.resize(
//                 positions.size(),
//                 glm::vec2(0.0f)
//             );
//         }


//         // ----------------------------------------------------
//         // JOINTS_0
//         // ----------------------------------------------------

//         std::vector<glm::ivec4> boneIDs;


//         if (
//             attributes.contains(
//                 "JOINTS_0"
//             )
//         )
//         {
//             unsigned int jointAccInd =
//                 attributes["JOINTS_0"];


//             boneIDs =
//                 getJointIndices(
//                     JSON["accessors"][jointAccInd]
//                 );
//         }


//         if (
//             boneIDs.size() !=
//             positions.size()
//         )
//         {
//             boneIDs.resize(
//                 positions.size(),
//                 glm::ivec4(0)
//             );
//         }


//         // ----------------------------------------------------
//         // WEIGHTS_0
//         // ----------------------------------------------------

//         std::vector<glm::vec4> weights;


//         if (
//             attributes.contains(
//                 "WEIGHTS_0"
//             )
//         )
//         {
//             unsigned int weightAccInd =
//                 attributes["WEIGHTS_0"];


//             std::vector<float> weightVec =
//                 getFloats(
//                     JSON["accessors"][weightAccInd]
//                 );


//             weights =
//                 groupFloatsVec4(
//                     weightVec
//                 );
//         }


//         if (
//             weights.size() !=
//             positions.size()
//         )
//         {
//             weights.resize(
//                 positions.size(),
//                 glm::vec4(0.0f)
//             );
//         }


//         // ----------------------------------------------------
//         // Indices
//         // ----------------------------------------------------

//         std::vector<GLuint> indices;


//         if (
//             primitive.contains(
//                 "indices"
//             )
//         )
//         {
//             unsigned int indAccInd =
//                 primitive["indices"];


//             indices =
//                 getIndices(
//                     JSON["accessors"][indAccInd]
//                 );
//         }
//         else
//         {
//             indices.resize(
//                 positions.size()
//             );


//             for (
//                 unsigned int i = 0;
//                 i < positions.size();
//                 i++
//             )
//             {
//                 indices[i] =
//                     i;
//             }
//         }


//         // ----------------------------------------------------
//         // Textures
//         // ----------------------------------------------------

//         std::vector<Texture> textures =
//             getTextures(
//                 primitive
//             );


//         // ----------------------------------------------------
//         // Build vertices
//         // ----------------------------------------------------

//         std::vector<Vertex> vertices =
//             assembleVertices(
//                 positions,
//                 normals,
//                 texUVs,
//                 boneIDs,
//                 weights
//             );


//         // ----------------------------------------------------
//         // Create Mesh
//         // ----------------------------------------------------

//         meshes.push_back(
//             Mesh(
//                 vertices,
//                 indices,
//                 textures,
//                 instancing,
//                 instanceMatrix
//             )
//         );
//     }
// }


// void Model::traverseNode(unsigned int nextNode, glm::mat4 matrix)
// {
// 	// Current node
// 	json node = JSON["nodes"][nextNode];

// 	// Get translation if it exists
// 	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
// 	if (node.find("translation") != node.end())
// 	{
// 		float transValues[3];
// 		for (unsigned int i = 0; i < node["translation"].size(); i++)
// 			transValues[i] = (node["translation"][i]);
// 		translation = glm::make_vec3(transValues);
// 	}
// 	// Get quaternion if it exists
// 	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
// 	if (node.find("rotation") != node.end())
// 	{
// 		float rotValues[4] =
// 		{
// 			node["rotation"][3],
// 			node["rotation"][0],
// 			node["rotation"][1],
// 			node["rotation"][2]
// 		};
// 		rotation = glm::make_quat(rotValues);
// 	}
// 	// Get scale if it exists
// 	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
// 	if (node.find("scale") != node.end())
// 	{
// 		float scaleValues[3];
// 		for (unsigned int i = 0; i < node["scale"].size(); i++)
// 			scaleValues[i] = (node["scale"][i]);
// 		scale = glm::make_vec3(scaleValues);
// 	}
// 	// Get matrix if it exists
// 	glm::mat4 matNode = glm::mat4(1.0f);
// 	if (node.find("matrix") != node.end())
// 	{
// 		float matValues[16];
// 		for (unsigned int i = 0; i < node["matrix"].size(); i++)
// 			matValues[i] = (node["matrix"][i]);
// 		matNode = glm::make_mat4(matValues);
// 	}

// 	// Initialize matrices
// 	glm::mat4 trans = glm::mat4(1.0f);
// 	glm::mat4 rot = glm::mat4(1.0f);
// 	glm::mat4 sca = glm::mat4(1.0f);

// 	// Use translation, rotation, and scale to change the initialized matrices
// 	trans = glm::translate(trans, translation);
// 	rot = glm::mat4_cast(rotation);
// 	sca = glm::scale(sca, scale);

// 	// Multiply all matrices together
// 	glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;

// 	// Check if the node contains a mesh and if it does load it
// 	if (node.find("mesh") != node.end())
// 	{
// 		translationsMeshes.push_back(translation);
// 		rotationsMeshes.push_back(rotation);
// 		scalesMeshes.push_back(scale);
// 		matricesMeshes.push_back(matNextNode);

// 		loadMesh(node["mesh"]);
// 	}

// 	// Check if the node has children, and if it does, apply this function to them with the matNextNode
// 	if (node.find("children") != node.end())
// 	{
// 		for (unsigned int i = 0; i < node["children"].size(); i++)
// 			traverseNode(node["children"][i], matNextNode);
// 	}
// }

// std::vector<unsigned char> Model::getData()
// {
// 	// Create a place to store the raw text, and get the uri of the .bin file
// 	std::string bytesText;
// 	std::string uri = JSON["buffers"][0]["uri"];

//     try
//     {
// 	    // Store raw text data into bytesText
// 	    std::string fileStr = std::string(file);
// 	    std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
// 	    bytesText = get_file_contents((fileDirectory + uri).c_str());
//     }
//     catch (const nlohmann::json::exception& e)
//     {
//         std::cerr << "JSON exception: " << e.what() << std::endl; throw;
//     }
    
// 	// Transform the raw text data into bytes and put them in a vector
// 	std::vector<unsigned char> data(bytesText.begin(), bytesText.end());
// 	return data;
// }

// std::vector<float> Model::getFloats(json accessor)
// {
// 	std::vector<float> floatVec;

//     // for (int i = 0; i < JSON["accessors"].size(); i++)
//     // {
//     //     std::cout << "Accessor " << i << ":\n";
//     //     std::cout << JSON["accessors"][i].dump(2) << "\n\n";
//     // }

//     // for (int i = 0; i < JSON["bufferViews"].size(); i++)
//     // {
//     //     std::cout << "BufferView " << i << ":\n";
//     //     std::cout << JSON["bufferViews"][i].dump(2) << "\n\n";
//     // }

//     // std::cout << "\n=== getFloats() ===\n";
//     // std::cout << accessor.dump(2) << std::endl;

// 	// std::cout << "Reading bufferView index...\n";
//     unsigned int buffViewInd = accessor.value("bufferView", 1);

//     // std::cout << "Reading count...\n";
//     unsigned int count = accessor["count"];

//     // std::cout << "Reading accessor byteOffset...\n";
//     unsigned int accByteOffset = accessor.value("byteOffset", 0);

//     // std::cout << "Reading type...\n";
//     std::string type = accessor["type"];

//     // std::cout << "Loading bufferView...\n";
//     json bufferView = JSON["bufferViews"][buffViewInd];

//     // std::cout << "Reading bufferView byteOffset...\n";
//     unsigned int byteOffset = bufferView.value("byteOffset", 0);

//     // std::cout << "Done reading JSON.\n";

// 	// Interpret the type and store it into numPerVert
// 	unsigned int numPerVert;
// 	if (type == "SCALAR") numPerVert = 1;
// 	else if (type == "VEC2") numPerVert = 2;
// 	else if (type == "VEC3") numPerVert = 3;
// 	else if (type == "VEC4") numPerVert = 4;
// 	else throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3, or VEC4)");

// 	// Go over all the bytes in the data at the correct place using the properties from above
// 	unsigned int beginningOfData = byteOffset + accByteOffset;
// 	unsigned int lengthOfData = count * 4 * numPerVert;
// 	for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i += 4)
// 	{
// 		unsigned char bytes[] = { data[i], data[i + 1], data[i + 2], data[i + 3] };
// 		float value;
// 		std::memcpy(&value, bytes, sizeof(float));
// 		floatVec.push_back(value);
// 	}

// 	return floatVec;
// }

// std::vector<GLuint> Model::getIndices(json accessor)
// {
// 	std::vector<GLuint> indices;

// 	// Get properties from the accessor
// 	unsigned int buffViewInd = accessor.value("bufferView", 0);
// 	unsigned int count = accessor["count"];
// 	unsigned int accByteOffset = accessor.value("byteOffset", 0);
// 	unsigned int componentType = accessor["componentType"];

// 	// Get properties from the bufferView
// 	json bufferView = JSON["bufferViews"][buffViewInd];
// 	unsigned int byteOffset = bufferView.value("byteOffset", 0);

// 	// Get indices with regards to their type: unsigned int, unsigned short, or short
// 	unsigned int beginningOfData = byteOffset + accByteOffset;
// 	if (componentType == 5125)
// 	{
// 		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4; i += 4)
// 		{
// 			unsigned char bytes[] = { data[i], data[i + 1], data[i + 2], data[i + 3] };
// 			unsigned int value;
// 			std::memcpy(&value, bytes, sizeof(unsigned int));
// 			indices.push_back((GLuint)value);
// 		}
// 	}
// 	else if (componentType == 5123)
// 	{
// 		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i += 2)
// 		{
// 			unsigned char bytes[] = { data[i], data[i + 1] };
// 			unsigned short value;
// 			std::memcpy(&value, bytes, sizeof(unsigned short));
// 			indices.push_back((GLuint)value);
// 		}
// 	}
// 	else if (componentType == 5122)
// 	{
// 		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i += 2)
// 		{
// 			unsigned char bytes[] = { data[i], data[i + 1] };
// 			short value;
// 			std::memcpy(&value, bytes, sizeof(short));
// 			indices.push_back((GLuint)value);
// 		}
// 	}

// 	return indices;
// }

// std::vector<Texture> Model::getTextures(const json& primitive)
// {
// 	std::vector<Texture> textures;

// 	std::string fileStr = std::string(file);
// 	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

// 	// Go over all images
// 	for (unsigned int i = 0; i < JSON["images"].size(); i++)
// 	{
// 		// uri of current texture
// 		std::string texPath = JSON["images"][i]["uri"];

// 		// Check if the texture has already been loaded
// 		bool skip = false;
// 		for (unsigned int j = 0; j < loadedTexName.size(); j++)
// 		{
// 			if (loadedTexName[j] == texPath)
// 			{
// 				textures.push_back(loadedTex[j]);
// 				skip = true;
// 				break;
// 			}
// 		}

// 		// If the texture has been loaded, skip this
// 		if (!skip)
// 		{
//             if (texPath.find("baseColor") != std::string::npos)
//             {
//                 Texture diffuse = Texture((fileDirectory + texPath).c_str(), "diffuse", loadedTex.size());
// 				textures.push_back(diffuse);
// 				loadedTex.push_back(diffuse);
// 				loadedTexName.push_back(texPath);
//             }

// 			// // Load diffuse texture
// 			// if (texPath.find("baseColor") != std::string::npos)
// 			// {
// 			// 	Texture diffuse = Texture((fileDirectory + texPath).c_str(), "diffuse", loadedTex.size());
// 			// 	textures.push_back(diffuse);
// 			// 	loadedTex.push_back(diffuse);
// 			// 	loadedTexName.push_back(texPath);
// 			// }
// 			// // Load specular texture
// 			// else if (texPath.find("metallicRoughness") != std::string::npos)
// 			// {
// 			// 	Texture specular = Texture((fileDirectory + texPath).c_str(), "specular", loadedTex.size());
// 			// 	textures.push_back(specular);
// 			// 	loadedTex.push_back(specular);
// 			// 	loadedTexName.push_back(texPath);
// 			// }
// 		}
// 	}

// 	return textures;
// }

// std::vector<Vertex> Model::assembleVertices
// (
// 	std::vector<glm::vec3> positions,
// 	std::vector<glm::vec3> normals,
// 	std::vector<glm::vec2> texUVs
// )
// {
//     // std::cout << "Positions: " << positions.size() << '\n';
//     // std::cout << "Normals:   " << normals.size() << '\n';
//     // std::cout << "UVs:       " << texUVs.size() << '\n';

// 	std::vector<Vertex> vertices;
// 	for (int i = 0; i < positions.size(); i++)
// 	{
// 		vertices.push_back
// 		(
// 			Vertex
// 			{
// 				positions[i],
// 				normals[i],
// 				glm::vec3(1.0f, 1.0f, 1.0f),
// 				texUVs[i]
// 			}
// 		);
// 	}

//     // for (int i = 0; i < 10 && i < texUVs.size(); i++)
//     // {
//     //     std::cout << i << ": "
//     //             << texUVs[i].x << ", "
//     //             << texUVs[i].y << '\n';
//     // }
    
// 	return vertices;
// }

// std::vector<glm::vec2> Model::groupFloatsVec2(std::vector<float> floatVec)
// {
// 	const unsigned int floatsPerVector = 2;
	
// 	std::vector<glm::vec2> vectors;
// 	for (unsigned int i = 0; i < floatVec.size(); i += floatsPerVector)
// 	{
// 		vectors.push_back(glm::vec2(0, 0));

// 		for (unsigned int j = 0; j < floatsPerVector; j++)
// 		{
// 			vectors.back()[j] = floatVec[i + j];
// 		}
// 	}
// 	return vectors;
// }
// std::vector<glm::vec3> Model::groupFloatsVec3(std::vector<float> floatVec)
// {
// 	const unsigned int floatsPerVector = 3;

// 	std::vector<glm::vec3> vectors;
// 	for (unsigned int i = 0; i < floatVec.size(); i += floatsPerVector)
// 	{
// 		vectors.push_back(glm::vec3(0, 0, 0));

// 		for (unsigned int j = 0; j < floatsPerVector; j++)
// 		{
// 			vectors.back()[j] = floatVec[i + j];
// 		}
// 	}
// 	return vectors;
// }
// std::vector<glm::vec4> Model::groupFloatsVec4(std::vector<float> floatVec)
// {
// 	const unsigned int floatsPerVector = 4;

// 	std::vector<glm::vec4> vectors;
// 	for (unsigned int i = 0; i < floatVec.size(); i += floatsPerVector)
// 	{
// 		vectors.push_back(glm::vec4(0, 0, 0, 0));

// 		for (unsigned int j = 0; j < floatsPerVector; j++)
// 		{
// 			vectors.back()[j] = floatVec[i + j];
// 		}
// 	}
// 	return vectors;
// }