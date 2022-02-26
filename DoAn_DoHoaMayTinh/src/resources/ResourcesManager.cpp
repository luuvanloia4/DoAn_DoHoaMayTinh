#include "ResourcesManager.h"

RawTexture& ResourcesManager::LoadTexture(std::string path) {
    if (m_Textures.find(path) == m_Textures.end()) {
        m_Textures.try_emplace(path, path);
    }

    return m_Textures.at(path);
}

RawModel& ResourcesManager::LoadModel(std::string path) {
    if (m_Models.find(path) == m_Models.end()) {
        m_Models.try_emplace(path, path, *this);
    }

    return m_Models.at(path);
}

//Sound& ResourcesManager::LoadSound(std::string path) {
//    if (m_Sounds.find(path) == m_Sounds.end()) {
//        m_Sounds.try_emplace(path, path);
//    }
//
//    return m_Sounds.at(path);
//}
