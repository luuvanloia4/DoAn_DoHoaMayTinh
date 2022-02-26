#include "World.h"

World::World(class Object* player, unsigned int neighbours_count)
    : m_Player(player)
    , m_NeighboursCountSqrt(std::sqrt(neighbours_count)) {
    m_CurrentChunkNeighbours.reserve(neighbours_count);
}

void World::Initialize() {
    UpdateChunks(glm::ivec2(0, 0));
    LoadChunks();
    RegisterUpdateCall();
}

void World::Update() {
    const glm::ivec2 closest_origin = ClosestChunkOrigin(m_Player->Root().Position());

    if (m_CurrentChunk->ChunkIndex() != closest_origin) {
        UnloadChunks();
        UpdateChunks(closest_origin);
        LoadChunks();
    }
}

void World::Destroy() {
    UnloadChunks();
}

void World::UpdateChunks(glm::ivec2 center) {
    m_CurrentChunkNeighbours.clear();

    // Generate grid of chunks
    const int half_size = m_NeighboursCountSqrt / 2;
    const int side = static_cast<int>(m_NeighboursCountSqrt) - half_size;
    for (int row = -half_size; row < side; row++) {
        for (int col = -half_size; col < side; col++) {
            glm::ivec2 origin(row, col);

            FixedChunk* chunk = GenerateChunk(origin + center);
            m_CurrentChunkNeighbours.push_back(chunk);
        }
    }

    // Center chunk is the m_CurrentChunk
    m_CurrentChunk = &m_World.at(center);
    m_CurrentChunkNeighbours.erase(m_CurrentChunkNeighbours.begin() + m_CurrentChunkNeighbours.size() / 2);
}

void World::LoadChunks() {
    LoadChunk(m_CurrentChunk);
    for (auto chunk = m_CurrentChunkNeighbours.begin(); chunk != m_CurrentChunkNeighbours.end(); chunk++) {
        LoadChunk(*chunk);
    }
}

void World::LoadChunk(FixedChunk* to_load) {
    Object().Scene().RegisterDrawCall(to_load, EShaderType::PureTexture);
    Object().Scene().AddRigidBody(to_load->PhysicsHandler());
}

void World::UnloadChunks() {
    UnloadChunk(m_CurrentChunk);
    for (auto chunk = m_CurrentChunkNeighbours.begin(); chunk != m_CurrentChunkNeighbours.end(); chunk++) {
        UnloadChunk(*chunk);
    }
}

void World::UnloadChunk(FixedChunk* to_unload) {
    Object().Scene().UnregisterDrawCall(to_unload, EShaderType::PureTexture);
    Object().Scene().RemoveRigidBody(to_unload->PhysicsHandler());
}

World::FixedChunk* World::GenerateChunk(glm::ivec2 origin) {
    if (m_World.find(origin) == m_World.cend()) {
        m_World.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(origin),
            std::forward_as_tuple(
                origin,
                Object().Scene().LoadTexture(GroundTexture),
                -1.0f,
                1.0f
            )
        );
    }

    return &m_World.at(origin);
}

glm::ivec2 World::ClosestChunkOrigin(const glm::vec3& position) const {
    return glm::ivec2(
        std::floor(position.x / static_cast<float>(ChunkWidth)),
        std::floor(position.z / static_cast<float>(ChunkLength))
    );
}