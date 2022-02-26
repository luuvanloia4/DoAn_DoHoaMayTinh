#ifndef TriggerOut_h
#define TriggerOut_h

#include "ConnectionsManager.h"

class TriggerOut final : public AbstractTriggerOut {
    friend class ConnectionsManager;

public:
    TriggerOut(Component* owner)
        : m_Owner(owner)
        , m_ConnectionsManager(nullptr) {}
    TriggerOut(const TriggerOut&) = delete;
    TriggerOut& operator=(const TriggerOut& other) = delete;
    TriggerOut(TriggerOut&&) = delete;
    TriggerOut& operator=(TriggerOut&&) = delete;
    ~TriggerOut() = default;

    Component* Owner() const override { return m_Owner; }

    void Trigger() override { 
        if (m_ConnectionsManager) {
            m_ConnectionsManager->ForwardTrigger(this);
        }
    }

private:
    Component* m_Owner;
    ConnectionsManager* m_ConnectionsManager;
};

#endif
