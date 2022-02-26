#ifndef Mesh_h
#define Mesh_h

#include "../../resources/RawModel/RawMesh.h"
#include "../Texture.h"

#pragma warning(push, 0)
#include <glad/glad.h>
#pragma warning(pop)

#include <vector>

class Mesh {
public:
    explicit Mesh(const RawMesh& raw_mesh);

    Mesh() = delete;
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;
    ~Mesh();

    GLuint VAO() const { return m_VAO; }
    GLuint VBO() const { return m_VBO; }
    GLuint EBO() const { return m_EBO; }
    GLsizei IndicesCount() const { return m_IndicesCount; }
    const Texture* Diffuse() const { return m_Diffuse.get(); }
    const Texture* Specular() const { return m_Specular.get(); }
    float Shininess() const { return m_Shininess; }

private:
    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_EBO;
    GLsizei m_IndicesCount;
    std::unique_ptr<Texture> m_Diffuse{ nullptr };
    std::unique_ptr<Texture> m_Specular{ nullptr };
    float m_Shininess;
};

#endif