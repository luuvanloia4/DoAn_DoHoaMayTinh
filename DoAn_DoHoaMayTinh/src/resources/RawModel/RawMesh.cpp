#include "RawMesh.h"

RawMesh::RawMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, const RawTexture* diffuse, const RawTexture* specular, float shininess)
    : m_Vertices(vertices)
    , m_Indices(indices)
    , m_Diffuse(diffuse)
    , m_Specular(specular)
    , m_Shininess(shininess) {
}
