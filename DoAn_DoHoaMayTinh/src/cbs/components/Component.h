#ifndef Component_h
#define Component_h

#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>

class Object;
class ConnectionPipe;
class ConnectionsManager;

/** \brief Base class for all components
 *
 * Components represents behaviour of game objects. To create new component inherit from this class and override
 * all neccesary functions. Each component has unique ID of type ID_t. 
 */
class Component {
    friend class Object;

public:
    using ID_t = std::uint8_t;

    Component() = default;
    Component(const Component&) = delete;
    Component(Component&&) = delete;
    virtual ~Component() = default;

    /// Returns component ID
    ID_t ID() const { return m_ID; }

    /// Returns component owner
    Object& Object() const { return *m_Object; }

protected:
    virtual void Initialize() {};
    virtual void Update() {};
    virtual void Destroy() {};

    void RegisterUpdateCall() const;
    void UnregisterUpdateCall() const;

private:
    class Object* m_Object{ nullptr };
    ID_t m_ID{ 0 };
};

#endif
