#ifndef RawModel_h
#define RawModel_h

#include "RawMesh.h"

#pragma warning(push, 0)
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#pragma warning(pop)

#include <vector>

class ResourcesManager;

class RawModel {
public:
    RawModel(std::string path, ResourcesManager& manager);

    RawModel() = delete;
    RawModel(const RawModel&) = default;
    RawModel& operator=(const RawModel&) = default;
    RawModel(RawModel&&) = default;
    RawModel& operator=(RawModel&&) = default;
    ~RawModel() = default;

    const std::string& Path() const { return m_Path; }
    const std::vector<RawMesh>& RawMeshes() const { return m_RawMeshes; }

    void Skin(const std::string& material_path);
    void Skin(const RawTexture* diffuse, const RawTexture* specular, float shininess);

private:
    void LoadNode(const aiNode* node, const aiScene* scene, const std::string& directory, ResourcesManager& manager);
    void LoadMesh(const aiMesh* mesh, const aiScene* scene, const std::string& directory, ResourcesManager& manager);

    std::string m_Path;
    std::vector<RawMesh> m_RawMeshes;
};

#endif 
