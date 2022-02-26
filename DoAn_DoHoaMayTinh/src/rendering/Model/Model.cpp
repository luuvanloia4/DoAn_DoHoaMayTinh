#include "Model.h"

Model::Model(const RawModel& raw_model) {
    m_Meshes.reserve(raw_model.RawMeshes().size());

    auto end = raw_model.RawMeshes().end();
    for (auto it = raw_model.RawMeshes().begin(); it != end; it++) {
        m_Meshes.emplace_back(*it);
    }
}
