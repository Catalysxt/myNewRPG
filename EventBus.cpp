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
// Adds a handler function that will be called whenever an event of
// the specified type is published.
// ============================================================================

void EventBus::Subscribe(EventType type, EventHandler handler) {
    m_Subscribers[type].push_back(std::move(handler));
}

// ============================================================================
// PUBLISH
// ============================================================================
// Notifies all subscribers of the given event type.
// Each subscriber's handler function is called with the event data.
// ============================================================================

void EventBus::Publish(const GameEvent& event) {
    auto it = m_Subscribers.find(event.type);
    if (it != m_Subscribers.end()) {
        for (const auto& handler : it->second) {
            handler(event);
        }
    }
}

// ============================================================================
// CLEAR ALL
// ============================================================================
// Removes all subscribers. Useful for testing or game reset.
// ============================================================================

void EventBus::ClearAll() {
    m_Subscribers.clear();
}
