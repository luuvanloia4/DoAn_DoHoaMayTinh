#include "Mesh.h"

Mesh::Mesh(const RawMesh& raw_mesh) 
    : m_IndicesCount(raw_mesh.Indices().size())
    , m_Shininess(static_cast<float>(raw_mesh.Shininess())) {
    if (raw_mesh.Diffuse()) {
        m_Diffuse = std::make_unique<Texture>(*raw_mesh.Diffuse(), Texture::EType::Diffuse);
    }

    if (raw_mesh.Specular()) {
        m_Specular = std::make_unique<Texture>(*raw_mesh.Specular(), Texture::EType::Specular);
    }

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, raw_mesh.Vertices().size() * sizeof(RawMesh::Vertex), &raw_mesh.Vertices()[0], GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(RawMesh::Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal vectors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(RawMesh::Vertex), (void*)offsetof(RawMesh::Vertex, RawMesh::Vertex::Normal));
    glEnableVertexAttribArray(1);

    // RawTexture coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(RawMesh::Vertex), (void*)offsetof(RawMesh::Vertex, RawMesh::Vertex::TexCoords));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndicesCount * sizeof(unsigned int), &raw_mesh.Indices()[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

Mesh::Mesh(Mesh&& other) noexcept
    : m_VAO(std::exchange(other.m_VAO, 0))
    , m_VBO(std::exchange(other.m_VBO, 0))
    , m_EBO(std::exchange(other.m_EBO, 0))
    , m_Diffuse(std::move(other.m_Diffuse)) 
    , m_Specular(std::move(other.m_Specular)) {
    m_IndicesCount = other.m_IndicesCount;
    m_Shininess = other.m_Shininess;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    m_VAO = std::exchange(other.m_VAO, 0);
    m_VBO = std::exchange(other.m_VBO, 0);
    m_EBO = std::exchange(other.m_EBO, 0);
    m_Diffuse = std::move(other.m_Diffuse);
    m_Specular = std::move(other.m_Diffuse);
    m_IndicesCount = other.m_IndicesCount;
    m_Shininess = other.m_Shininess;

    return *this;
}
