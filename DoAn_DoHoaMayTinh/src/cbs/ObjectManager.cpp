#include "ObjectManager.h"

#include "../scenes/Scene.h"

ObjectManager::ObjectManager(class Scene& owner)
    : m_Scene(owner)
    , m_ToDestroy(0)
    , m_Iterator(0) {
}

void ObjectManager::ProcessFrame() {
    // 
    Objects_t::size_type to_initialize = m_ToInitializeNextFrame;
    m_ToInitializeNextFrame = 0;

    // Update components
    m_Iterator = m_ToDestroy;
    for (; m_Iterator < m_Objects.size() - to_initialize; m_Iterator++) {
        m_Objects[m_Iterator]->ProcessFrame();
    }

    // Destroy components
    if (m_ToDestroy > 0) {
        m_Iterator = 0;
        for (; m_Iterator < m_ToDestroy; m_Iterator++) {
            m_Objects[m_Iterator]->DestroyComponents();
        }
        m_Objects.erase(m_Objects.begin(), m_Objects.begin() + m_ToDestroy);
        m_ToDestroy = 0;
    }
}

void ObjectManager::DestroyObjects() {
    for (auto& object : m_Objects) {
        object->DestroyComponents();
    }
    m_Objects.clear();
}

Object* ObjectManager::CreateObject(const std::string& name) {
    m_Objects.emplace_back(std::make_unique<Object>(*this, m_NextObjectID++, name));
    return m_Objects.back().get();
}

void ObjectManager::DestroyObject(Object::ID_t id) {
    auto object = std::find_if(m_Objects.begin(),
                               m_Objects.end(),
                               [=](auto& it){ return it->ID() == id; });

    if (object != m_Objects.end() && std::distance(m_Objects.begin(), object) > static_cast<ptrdiff_t>(m_ToDestroy)) {
        std::iter_swap(m_Objects.begin() + m_ToDestroy, object);
        m_ToDestroy += 1;
    }
}
