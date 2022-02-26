#ifndef ObjectManager_h
#define ObjectManager_h

#include "Object.h"

#include <vector>
#include <string>

class Scene;

class ObjectManager {
    using Objects_t = std::vector<std::unique_ptr<Object>>;

public:
    explicit ObjectManager(class Scene& owner);

    void ProcessFrame();
    void DestroyObjects();

    Object* CreateObject(const std::string& name);
    void DestroyObject(Object::ID_t id);

    Scene& Scene() const { return m_Scene; }

private:
    class Scene& m_Scene;

    Object::ID_t m_NextObjectID;

    Objects_t m_Objects;
    Objects_t::size_type m_ToDestroy;
    Objects_t::size_type m_ToInitializeNextFrame;

    Objects_t::size_type m_Iterator;
};

#endif
