#ifndef World_h
#define World_h

#include "../Component.h"
#include "../../Object.h"
#include "../../../scenes/Scene.h"
#include "../../connections/PropertyOut.h"
#include "../../../resources/Sound.h"

#include "Chunk.h"

#include <string>
#include <vector>
#include <unordered_map>

constexpr size_t ChunkWidth = 32;
constexpr size_t ChunkLength = 32;
#define GroundTexture "resources/textures/ground.jpg"

class World : public Component {
    using FixedChunk = Chunk<ChunkWidth, ChunkLength>;

public:
    World(class Object* player, unsigned int neighbours_count);

    void Initialize() override;
    void Update() override;
    void Destroy() override;

    PropertyOut<Sound*> Background{ this, nullptr };

private:
    struct IVec2Key {
        size_t operator()(const glm::ivec2& k)const {
            return std::hash<int>()(k.x) ^ std::hash<int>()(k.y);
        }

        bool operator()(const glm::ivec2& a, const glm::ivec2& b)const {
            return a.x == b.x && a.y == b.y;
        }
    };

    void UpdateChunks(glm::ivec2 center);
    void LoadChunks();
    void LoadChunk(FixedChunk* to_load);
    void UnloadChunks();
    void UnloadChunk(FixedChunk* to_unload);
    World::FixedChunk* GenerateChunk(glm::ivec2 origin);
    glm::ivec2 ClosestChunkOrigin(const glm::vec3& position) const;

    class Object* m_Player;

    std::unordered_map<glm::ivec2, Chunk<ChunkWidth, ChunkLength>, World::IVec2Key> m_World;
    FixedChunk* m_CurrentChunk{ nullptr };
    std::vector<FixedChunk*> m_CurrentChunkNeighbours;
    unsigned int m_NeighboursCountSqrt;
};

#endif
