#ifndef TriggerIn_h
#define TriggerIn_h

#include "ConnectionsManager.h"

template <class O, void(O::* F)()>
class TriggerIn final : public AbstractTriggerIn {
    friend class ConnectionsManager;

public:
    TriggerIn(O* owner)
        : m_Owner(owner) {}

    TriggerIn() = delete;
    TriggerIn(const TriggerIn&) = delete;
    TriggerIn& operator=(const TriggerIn& other) = delete;
    TriggerIn(TriggerIn&&) = delete;
    TriggerIn& operator=(TriggerIn&&) = default;
    ~TriggerIn() = default;

    Component* Owner() const { return m_Owner; }

    void Receive() override { (m_Owner->*F)(); }

private:
    O* m_Owner;
};

#endif
