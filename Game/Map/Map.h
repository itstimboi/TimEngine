#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "Mesh/Mesh.h"
#include "MapFormat.h"

using MapHandle = unsigned int;

// ------------------------------------------------------------
// Map
// ------------------------------------------------------------

class Map
{
public:

    static bool Init();

    static void Shutdown();

    static MapHandle Pre_CacheMap(
        const std::string& path
    );

    static void Draw(
        MapHandle map,
        TE::Shader& shader,
        Camera& camera
    );


    Map(
        const char* file
    );


    bool Load(
        const std::string& filename
    );


    void New(
        const std::string& name
    );

    

    MapData& GetData();

    const MapData& GetData() const;


private:

    bool BuildMeshes();


    std::string file;

    MapData data;

    std::vector<Mesh> meshes;


    static std::vector<Map*> mapCache;
};