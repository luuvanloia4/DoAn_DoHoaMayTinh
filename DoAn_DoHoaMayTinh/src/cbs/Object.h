#ifndef Object_h
#define Object_h

#include "connections/ConnectionsManager.h"
#include "components/Component.h"
#include "components/Transform.h"

#pragma warning(push, 0)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#pragma warning(pop)

#include <assert.h>
#include <string>
#include <vector>
#include <algorithm>

class ObjectManager;
class Scene;

class Object {
    using Components_t = std::vector<std::unique_ptr<Component>>;

public:
    using ID_t = std::size_t;

    Object(ObjectManager& owner, ID_t id, std::string name);

    Object() = delete;
    Object(const Object&) = delete;
    Object(Object&&) = delete;
    ~Object() = default;

    void ProcessFrame();
    void DestroyComponents();

    void RegisterUpdateCall(const Component* component);
    void UnregisterUpdateCall(const Component* component);

    ID_t ID() const { return m_ID; }
    const std::string& Name() const { return m_Name; }
    Transform& Root() { return m_Root; }
    Scene& Scene() const;

    /** \brief Create component.
     *
     * Creates component of type T with arguments Args and returns pointer of type T.
     * At the time of calling constructor new Component will not have access to it's Object-owner.
     * Futhermore component receives it's own ID greater than 0. All Object 
     * related function class e.g. Transform.Position() should happen inside Initialize function.
     * By default Transform component is marked as Root and has ID of 1.
     *
     * @param params All arguments to be forwarded to contstructor
     */
    template <class T, typename ...Args>
    T* CreateComponent(Args&&... params) {
        //Gọi hàm tạo compontent và thêm vào cuối vector m_Components
        m_Components.emplace_back(std::make_unique<T>(params...));

        //Liên kết ngược từ component đến object
        auto& comp = m_Components.back();
        comp->m_Object = this;
        comp->m_ID = m_NextCompID++;

        
        m_ToInitializeNextFrame += 1;
        // Trả về pointer trỏ đến phần tử vừa tạo
        return dynamic_cast<T*>(m_Components.back().get());
    }

    /** \brief Remove components.
     *
     * Mark component as to destroy and guarantees thier's Destory functions
     * will be called at the end of current frame. Destruction of object will
     * happen naturally after that.
     * Components to destory is specified by type and all components of this type will be destroyed.
     * Destroying component also removes all connections from and to this component.
     * Transform component by default cannot to be destroyed manually as it's
     * lifetime is bounded to Object lifetime.
     * Attempt to destroy non existing component will not have effect.
     */
    template <class T>
    void RemoveComponents() {
        for (auto& comp : m_Components) {
            if (dynamic_cast<T*>(comp->get()) != nullptr) {
                MarkToDestroy(comp);
            }
        }
    }

    /** \brief Remove component.
     *
     * Mark component as to destroy and guarantees thiers Destory functions
     * will be called at the end of current frame. Destruction of object will
     * happen naturally after that.
     * Components to destory can be specified either by ID. If type.
     * Destroying component also removes all connections from and to this component.
     * Transform component by default cannot to be destroyed manually as it's
     * lifetime is bounded to Object lifetime.
     * Attempt to destroy non existing component will not have effect.
     *
     * @param id ID of component to be destroyed
     */
    void RemoveComponent(Component::ID_t id) {
        assert(id > 1);
        
        auto comp = std::find_if(m_Components.begin(),
                                 m_Components.end(),
                                 [=](std::unique_ptr<Component>& comp) { return comp->ID() == id; });

        if (comp != m_Components.end()) {
            MarkToDestroy(comp);
        }
    }


    /** \brief Get components.
     *
     * Attempts to find components by type.
     * Returns vector of pointers of given type T. If no components found return vector of size 0.
     */
    template <class T>
    std::vector<T*> GetComponents() {
        std::vector<T*> comps;
        T* comp = nullptr;

        for (auto it = m_Components.begin(); it != m_Components.end(); it++) {
            Component* to_cast = it->get();
            comp = dynamic_cast<T*>(to_cast);

            if (comp != nullptr) {
                comps.push_back(comp);
            }
        }

        return comps;
    }

    /** \brief Get component.
     *
     * Attempts to find components by ID.
     * Returns pointer of given type T. If no components found returns null pointer.
     *
     * @param id ID of component to be destroyed.
     */
    template <class T>
    T* GetComponent(Component::ID_t id) {
        Components_t::iterator it = std::find_if(m_Components.begin(),
                               m_Components.end(),
                               [=](std::unique_ptr<Component>& curr) { return curr->ID() == id; });

        if (it != m_Components.end()) {
            return dynamic_cast<T*>(it->get());
        } else {
            return nullptr;
        }
    }

    template <class T>
    void Connect(PropertyOut<T>& subject, PropertyIn<T>& observer) {
        assert(subject.Owner()->Object().ID() == m_ID && observer.Owner()->Object().ID() == m_ID);
        m_ConnectionsManager.Connect(subject, observer);
    }

    template <class M, class O, void(O::* F)(M)>
    void Connect(MessageOut<M>& sender, MessageIn<M, O, F>& receiver) {
        assert(sender.Owner()->Object().ID() == m_ID && receiver.Owner()->Object().ID() == m_ID);
        m_ConnectionsManager.Connect(sender, receiver);
    }

    template <class O, void(O::*F)(void)>
    void Connect(TriggerOut& sender, TriggerIn<O, F>& receiver) {
        assert(sender.Owner()->Object().ID() == m_ID && receiver.Owner()->Object().ID() == m_ID);
        m_ConnectionsManager.Connect(sender, receiver);
    }

    template <class T>
    void Disconnect(PropertyOut<T>& subject, PropertyIn<T>& observer) {
        assert(subject.Owner()->Object().ID() == m_ID && observer.Owner()->Object().ID() == m_ID);
        m_ConnectionsManager.Disconnect(subject, observer);
    }

    template <class M, class O, void (O::*F)(M)>
    void Disconnect(MessageOut<M>& sender, MessageIn<M, O, F>& receiver) {
        assert(sender.Owner()->Object().ID() == m_ID && receiver.Owner()->Object().ID() == m_ID);
        m_ConnectionsManager.Disconnect(sender, receiver);
    }

    template <class O, void (O::*F)(void)>
    void Disconnect(TriggerOut& sender, TriggerIn<O, F>& receiver) {
        assert(sender.Owner()->Object().ID() == m_ID && receiver.Owner()->Object().ID() == m_ID);
        m_ConnectionsManager.Disconnect(sender, receiver);
    }

private:
    void MarkToDestroy(Components_t::iterator it);

    ID_t m_ID;
    std::string m_Name;

    ObjectManager& m_Owner;
    ConnectionsManager m_ConnectionsManager;

    Component::ID_t m_NextCompID;

    // All components owned by Object
    Transform m_Root;
    Components_t m_Components;
    Components_t::size_type m_ToDestroy;
    Components_t::size_type m_ToUpdate;
    Components_t::size_type m_ToInitializeNextFrame;

    Components_t::size_type m_Iterator;  // Process frame iterator
};

#endif
