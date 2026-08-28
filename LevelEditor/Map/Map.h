#pragma once

#include <string>
#include <cfloat>
#include <vector>

#include "MapFormat.h"

#include "../something.h"

#include <raylib/raylib.h>
#include <raylib/raymath.h>

class Map
{
public:

    Map();
    ~Map();


    // ========================================================
    // Map
    // ========================================================

    void New(
        const std::string& name = "Untitled Map"
    );

    bool Load(
        const std::string& filename
    );

    bool Save(
        const std::string& filename
    ) const;


    // ========================================================
    // Data
    // ========================================================

    EditorData& GetData();

    const EditorData& GetData() const;


    // ========================================================
    // Geometry
    // ========================================================

    void AddCube(
        const glm::vec3& min,
        const glm::vec3& max
    );


    // Add an edge, reusing an existing edge if possible.
    unsigned int AddEdge(
        unsigned int vertexA,
        unsigned int vertexB
    );

    // ========================================================
    // Rendering
    // ========================================================

    void Draw();

    bool BuildRenderMeshes();


    // ========================================================
    // Surface selection
    // ========================================================

    int PickSurface(
        const Ray& ray
    );

    int GetSelectedSurface() const;

    void SetSelectedSurface(
        int index
    );

    int GetHoveredSurface() const;

    void SetHoveredSurface(
        int index
    );

    void DrawSelection();


    // ========================================================
    // Vertex selection
    // ========================================================

    int PickVertex(
        const Ray& ray
    );

    int GetSelectedVertex() const;

    EditorVertex GetVertex(int index, const EditorData& data);

    EditorSurface GetSurface(int index, const EditorData& data);

    void SetSelectedVertex(
        int index
    );

    int GetHoveredVertex() const;

    void SetHoveredVertex(
        int index
    );

    void DrawSurfaceVertexSelection();


    // ========================================================
    // Vertex movement
    // ========================================================

    void MoveSelectedVertex(
        const glm::vec3& delta
    );

    void SetSelectedVertexPosition(
        const glm::vec3& position
    );

    glm::vec3 GetSelectedVertexPosition() const;


    // ========================================================
    // Surface movement
    // ========================================================

    void MoveSelectedSurface(
        const glm::vec3& delta
    );

    void RotateSelectedSurface(const glm::vec3& rotation);

    void SetSelectedSurfacePosition(
        const glm::vec3& position
    );

    glm::vec3 GetSelectedSurfacePosition() const;


    // ========================================================
    // Undo
    // ========================================================

    void Undo();


    // ========================================================
    // Deletion
    // ========================================================

    void DeleteSelectedVertex();

    void DeleteSelectedSurface();

    std::vector<unsigned int> GetSurfaceEdges(
        const EditorSurface& surface
    ) const;

    std::vector<unsigned int> GetEdgesUsingVertex(
        unsigned int vertex
    ) const;

    std::vector<unsigned int> GetSurfacesUsingEdge(
        unsigned int edge
    ) const;

    std::vector<unsigned int> GetSurfacesUsingVertex(
        unsigned int vertex
    ) const;

    int FindEdge(
        unsigned int vertex0,
        unsigned int vertex1
    ) const;

    void DrawOrphanedVertices();

    void DrawEdges();

    std::vector<unsigned int> GetSurfaceVertices(
        const EditorSurface& surface,
        const EditorData& data
    ) const;

    std::vector<unsigned int> GetSurfaceVertices(
        const EditorSurface& surface
    ) const;

    bool IsVertexUsed(
        unsigned int vertexIndex
    ) const;

    int PickEdge(const Ray& ray);

    int GetSelectedEdge() const;
    void SetSelectedEdge(int index);

    int GetHoveredEdge() const;
    void SetHoveredEdge(int index);

    void DrawEdgeSelection();

    int GetSelectedEntity() const;
    void SetSelectedEntity(int index);

    int GetHoveredEntity() const;
    void SetHoveredEntity(int index);

    int PickEntity(const Ray& ray);

    // ========================================================
    // Selection
    // ========================================================

    int selectedSurface = -1;
    int hoveredSurface = -1;

    int selectedVertex = -1;
    int hoveredVertex = -1;

    int selectedEdge = -1;
    int hoveredEdge = -1;

    int selectedEntity = -1;
    int hoveredEntity = -1;

    int selectedLight = -1;
    int hoveredLight = -1;

    int GetSelectedLight() const;
    void SetSelectedLight(int index);

    int GetHoveredLight() const;
    void SetHoveredLight(int index);

    int PickLight(const Ray& ray);

    void DrawLights(Camera camera);

    void AddLight(EditorLightType type);

    glm::vec3 selectedSurfaceRotation = glm::vec3(0.0f);
    glm::vec3 previousSurfaceRotation = glm::vec3(0.0f);

    void ScaleSelectedSurface(const glm::vec3& scale);

    void DrawEntities(Camera camera);

    std::vector<unsigned int> GetEdgeVertices(
        const EditorEdge& edge
    ) const;

    std::vector<unsigned int> GetEdgeVertices(
        const EditorEdge& edge,
        const EditorData& data
    ) const;

    void MoveSelectedEdge(const glm::vec3& delta);

    void MoveSelectedLight(
        const glm::vec3& delta
    );

    unsigned int AddVertex(
        const glm::vec3& position
    );

    unsigned int AddSurface(
        const std::vector<unsigned int>& vertexIndices,
        unsigned int material
    );

private:

    EditorData mapData;

    std::vector<EditorData> undoStack;

    // ========================================================
    // Undo
    // ========================================================

    void SaveUndoState();


    // ========================================================
    // Textures
    // ========================================================

    void UnloadTextures();


    void DrawThickFatyLine3D(const Vector3& start, const Vector3& end, float thickness, Color color);

    // ========================================================
    // Render data
    // ========================================================

    std::vector<::Mesh> renderMeshes;

    std::vector<::Material> renderMaterials;

    std::vector<Texture2D> textures;


    EditorEntity GetEntity(unsigned int type);

    Vector3 glmv3rayvec3(glm::vec3 value);
};