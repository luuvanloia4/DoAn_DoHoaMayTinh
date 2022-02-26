#ifndef Model_h
#define Model_h

#include "Mesh.h"
#include "../../resources/RawModel/RawModel.h"

#include <vector>

class Model {
public:
    explicit Model(const RawModel& raw_model);

    Model() = delete;
    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;
    Model(Model&&) = delete;
    Model& operator=(Model&&) = delete;
    ~Model() = default;

    const std::vector<Mesh>& Meshes() const { return m_Meshes; }

private:
    std::vector<Mesh> m_Meshes;
};

#endif
