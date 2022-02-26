#ifndef MessageIn_h
#define MessageIn_h

#include "ConnectionsManager.h"

template <class M, class O, void(O::*F)(M)>
class MessageIn final : public AbstractMessageIn {
    friend class ConnectionsManager;

public:
    MessageIn(O* owner)
        : m_Owner(owner) {}

    MessageIn() = delete;
    MessageIn(const MessageIn&) = delete;
    MessageIn& operator=(const MessageIn& other) = delete;
    MessageIn(MessageIn&&) = delete;
    MessageIn& operator=(MessageIn&&) = delete;
    ~MessageIn() = default;

    Component* Owner() const override { return m_Owner; }

    void Receive(void* message) override { (m_Owner->*F)(*static_cast<M*>(message)); }

private:
    O* m_Owner;
};

#endif
