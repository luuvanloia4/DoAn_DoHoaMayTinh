#ifndef Ground_h
#define Ground_h

#include "PerlinNoise.h"

#include "../Component.h"
#include "../../Object.h"
#include "../../connections/PropertyIn.h"
#include "../../../scenes/Scene.h"

#include "../../../rendering/IDrawable.h"
#include "../../../rendering/Texture.h"

#include "../../../physics/IPhysicalObject.h"

#include "../../../resources/RawTexture.h"

#include <array>

#pragma warning(push, 0)
#include <glm/gtc/type_ptr.hpp>
#pragma warning(pop)

constexpr float NoiseIncrement = 0.1f;

template <unsigned int Width, unsigned int Length>
class Chunk : public IDrawable, public IPhysicalObject {
    using Grid_t = std::array<std::array<float, Length + 1>, Width + 1>;

public:
    Chunk(const glm::ivec2& chunk_index, const RawTexture& texture, float min_height, float max_height);
    ~Chunk();

    void Draw(const ShaderProgram& shader) const override;

    void OnCollision(const btCollisionObject* collider) override {}
    void PhysicsUpdate() override {}

    glm::ivec2 ChunkIndex() { return m_ChunkIndex; }
    btRigidBody* PhysicsHandler() { return m_RigidBody; }

private:
    void InitRendering(const Grid_t& grid);
    void InitPhysics(const Grid_t& grid);

    glm::ivec2 m_ChunkIndex;    // Lower left corner
    PerlinNoise m_PerlinNoise;

    // Rendering
    GLuint m_ID;
    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_EBO;
    GLuint m_IndicesCount;
    std::unique_ptr<Texture> m_Texture;

    // Physics
    btRigidBody* m_RigidBody;
};

#endif

template<unsigned int Width, unsigned int Length>
inline Chunk<Width, Length>::Chunk(const glm::ivec2& chunk_index, const RawTexture& texture, float min_height, float max_height)
    : m_Texture(std::make_unique<Texture>(texture, Texture::EType::Diffuse))
    , m_ChunkIndex(chunk_index)
    , m_IndicesCount(Width* Length * 6) {

    const float x_offset = m_ChunkIndex.x * static_cast<int>(Width) * NoiseIncrement;
    const float z_offset = m_ChunkIndex.y * static_cast<int>(Length) * NoiseIncrement;

    Grid_t grid;
    for (int i = 0; i < Width + 1; i++) {
        for (int j = 0; j < Length + 1; j++) {

            int value = m_PerlinNoise.Noise(x_offset + i * NoiseIncrement, 0.0f /* ignoring height */, z_offset + j * NoiseIncrement);

            grid[i][j] = (value < min_height)? min_height: (value > max_height)? max_height: value;
        }
    }

    InitRendering(grid);
    InitPhysics(grid);
}

template<unsigned int Width, unsigned int Length>
inline Chunk<Width, Length>::~Chunk() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

template<unsigned int Width, unsigned int Length>
inline void Chunk<Width, Length>::Draw(const ShaderProgram& shader) const {
    shader.Uniform("model", glm::mat4(1.0f));

    glActiveTexture(GL_TEXTURE0);
    shader.Uniform("material.diffuse", 0);
    glBindTexture(GL_TEXTURE_2D, m_Texture->ID());

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_IndicesCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

template<unsigned int Width, unsigned int Length>
inline void Chunk<Width, Length>::InitRendering(const Chunk::Grid_t& grid) {
    const float x_offset = static_cast<float>(m_ChunkIndex.x * static_cast<int>(Width));
    const float z_offset = static_cast<float>(m_ChunkIndex.y * static_cast<int>(Length));

    const unsigned int rows = Width + 1;
    const unsigned int collumns = Length + 1;

    GLfloat *vertices = new GLfloat[rows * collumns * (3 + 2)];
    size_t index = 0; // vertices index
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < collumns; col++) {
            // Position
            vertices[index++] = row + x_offset;
            vertices[index++] = grid[row][col];
            vertices[index++] = col + z_offset;

            // Texture coordinates
            vertices[index++] = static_cast<float>(row) / static_cast<float>(Width);
            vertices[index++] = static_cast<float>(col) / static_cast<float>(Length);
        }
    }
    
    GLuint *indices = new GLuint[Width * Length * 6];
    size_t i_index = 0; // indices index
    for (int row = 0; row < Width; row++) {
        for (int col = 0; col < Length; col++) {
            // First
            indices[i_index++] = row * rows + col;
            indices[i_index++] = row * rows + col + 1;
            indices[i_index++] = (row + 1) * rows + col;

            // Second
            indices[i_index++] = row * rows + col + 1;
            indices[i_index++] = (row + 1) * rows + col + 1;
            indices[i_index++] = (row + 1) * rows + col;
        }
    }

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, rows * collumns * (3 + 2) * sizeof(GL_FLOAT), vertices, GL_STATIC_DRAW);

    // Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 5 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Width * Length * 6 * sizeof(GL_UNSIGNED_INT), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    delete[] vertices;
    delete[] indices;
}

template<unsigned int Width, unsigned int Length>
inline void Chunk<Width, Length>::InitPhysics(const Chunk::Grid_t& grid) {
    const btVector3 offset(m_ChunkIndex.x * static_cast<int>(Width), 0, m_ChunkIndex.y * static_cast<int>(Length));

    btTriangleMesh* mesh = new btTriangleMesh;
    for (size_t row = 0; row < Width; row++) {
        for (size_t col = 0; col < Length; col++) {
            // First triangle [upper-left, upper-right, lower-left]
            btVector3 f_v0(row, grid[row][col], col);
            btVector3 f_v1(row, grid[row][col + 1], col + 1);
            btVector3 f_v2(row + 1, grid[row + 1][col], col);
            mesh->addTriangle(f_v0 + offset, f_v1 + offset, f_v2 + offset);

            // Second triangle [upper-right, lower-right, lower-left]
            btVector3 s_v0(row, grid[row][col + 1], col + 1);
            btVector3 s_v1(row + 1, grid[row + 1][col + 1], col + 1);
            btVector3 s_v2(row + 1, grid[row + 1][col], col);
            mesh->addTriangle(s_v0 + offset, s_v1 + offset, s_v2 + offset);
        }
    }
    btBvhTriangleMeshShape* shape = new btBvhTriangleMeshShape(mesh, true);
    shape->setMargin(0.1f);

    btVector3 local_interia(0, 0, 0);
    btTransform start;
    start.setIdentity();

    btDefaultMotionState* motion_state = new btDefaultMotionState(start);
    btRigidBody::btRigidBodyConstructionInfo info(0.0f, motion_state, shape, local_interia);

    m_RigidBody = new btRigidBody(info);
    m_RigidBody->setUserPointer((IPhysicalObject*)this);
}
