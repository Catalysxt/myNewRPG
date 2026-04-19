#include "EventBus.h"

// ============================================================================
// SINGLETON INSTANCE
// ============================================================================
// Meyer's Singleton - thread-safe in C++11 and later.
// The static local variable is initialized exactly once, even in
// multi-threaded scenarios.
// ============================================================================

EventBus& EventBus::Instance() {
    static EventBus instance;
    return instance;
}

// ============================================================================
// SUBSCRIBE
// ============================================================================
// Creates a subscription token (shared_ptr<bool>) and stores the handler
// alongside a weak_ptr observer of that token. The caller holds the
// shared_ptr, which keeps the subscription alive. When the caller
// destroys or resets the token, the weak_ptr expires, and the handler
// will be cleaned up during the next Publish() call.
// ============================================================================

SubscriptionToken EventBus::Subscribe(EventType type, EventHandler handler) {
    auto token = std::make_shared<bool>(true); // Enable the subscription
    m_Subscribers[type].push_back({ token, std::move(handler) }); // Use aggregate initialization
    return token;
}

// ============================================================================
// PUBLISH
// ============================================================================
// Notifies all living subscribers of the given event type.
// After dispatching, dead subscriptions (whose tokens have expired)
// are swept from the vector using the erase-remove idiom.
// ============================================================================

void EventBus::Publish(const GameEvent& event) {
    auto it = m_Subscribers.find(event.type);
    if (it != m_Subscribers.end()) {
        auto& subs = it->second;

        // Dispatch to living handlers only
        for (const auto& sub : subs) {
            if (!sub.token.expired()) { // Check if object isn't destroyed
                sub.handler(event);
            }
        }

        // Cleanup: erase-remove dead subscriptions whose tokens have expired.
        // std::remove_if shifts living elements to the front and returns an
        // iterator to the new logical end. erase() then trims the dead tail.
        subs.erase(
            std::remove_if(subs.begin(), subs.end(),
                [](const Subscription& s) { return s.token.expired(); }),
            subs.end()
        );
    }
}

// ============================================================================
// CLEAR ALL
// ============================================================================
// Removes all subscribers. Useful for testing or game reset.
// Note: This does NOT invalidate tokens held by callers - those
// shared_ptrs remain valid but the handlers are simply gone.
// ============================================================================

void EventBus::ClearAll() {
    m_Subscribers.clear();
}
