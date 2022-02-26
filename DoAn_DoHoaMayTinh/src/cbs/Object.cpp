#include "Object.h"

#include "ObjectManager.h"
#include "../scenes/Scene.h"

Object::Object(ObjectManager& owner, ID_t id, std::string name)
    : m_ID(id)
    , m_Name(name)
    , m_Owner(owner)
    , m_ConnectionsManager()
    , m_NextCompID(2)
    , m_ToDestroy(0)
    , m_ToUpdate(0) 
    , m_ToInitializeNextFrame(0)
    , m_Iterator(0) {
    // Initialize Root component
    m_Root.m_Object = this;
    m_Root.m_ID = 1;
    m_Root.Identity();
    m_Root.Initialize();
}

void Object::ProcessFrame() {
    // 
    Components_t::size_type to_initialize = m_ToInitializeNextFrame;
    m_ToInitializeNextFrame = 0;

    // Initialize components
    m_Iterator = m_Components.size() - to_initialize;
    for (; m_Iterator < m_Components.size(); m_Iterator++) {
        m_Components[m_Iterator]->Initialize();
    }

    // Update components
    m_Iterator = m_ToDestroy;
    for (; m_Iterator < m_ToDestroy + m_ToUpdate; m_Iterator++) {
        m_Components[m_Iterator]->Update();
    }

    // Destroy components
    if (m_ToDestroy > 0) {
        m_Iterator = 0;
        for (; m_Iterator < m_ToDestroy; m_Iterator++) {
            m_ConnectionsManager.RemoveConnections(m_Components[m_Components.size() - 1 - m_Iterator].get());
            m_Components[m_Iterator]->Destroy();
        }
        m_Components.erase(m_Components.begin(), m_Components.begin() + m_ToDestroy);
        m_ToDestroy = 0;
    }
}

void Object::DestroyComponents() {
    for (auto& comp : m_Components) {
        comp->Destroy();
    }
    m_Components.clear();
}

void Object::RegisterUpdateCall(const Component* component) {
    assert(component->Object().ID() == m_ID);

    auto id = component->ID();
    Components_t::iterator comp = std::find_if(m_Components.begin(),
                                               m_Components.end(),
                                               [=](std::unique_ptr<Component>& curr) { return curr->ID() == id; });

    assert(comp != m_Components.end());
    if (std::distance(m_Components.begin() + m_ToDestroy, comp) >= static_cast<ptrdiff_t>(m_ToUpdate)) {
        std::iter_swap(m_Components.begin() + m_ToDestroy + m_ToUpdate, comp);
        m_ToUpdate += 1;
    }
}

void Object::UnregisterUpdateCall(const Component* component) {
    assert(component->Object().ID() == m_ID);

    auto id = component->ID();
    Components_t::iterator comp = std::find_if(m_Components.begin(),
                                               m_Components.end(),
                                               [=](std::unique_ptr<Component>& curr) { return curr->ID() == id; });

    assert(comp != m_Components.end());
    if (std::distance(m_Components.begin() + m_ToDestroy, comp) <= static_cast<ptrdiff_t>(m_ToUpdate)) {
        std::iter_swap(m_Components.begin() + m_ToDestroy + m_ToUpdate, comp);
        m_ToUpdate -= 1;
    }
}

Scene& Object::Scene() const {
    return m_Owner.Scene();
}

void Object::MarkToDestroy(Components_t::iterator it) {
    // Check if component hasn't been already marked
    if (std::distance(m_Components.begin(), it) > static_cast<ptrdiff_t>(m_ToDestroy)) {
        UnregisterUpdateCall(it->get());

        m_ToDestroy += 1;
        std::iter_swap(m_Components.begin() + m_ToDestroy, it);
    }
}
