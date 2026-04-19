#pragma once
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
#include <algorithm>
#include "GameEvent.h"

// ============================================================================
// SUBSCRIPTION TOKEN
// ============================================================================
// Returned by Subscribe(). The subscription stays alive as long as the caller
// holds this token. When the token is destroyed (goes out of scope or is
// reset), the EventBus will automatically clean up the dead handler during
// the next Publish() call.
//
// USAGE:
//   // Store the token to keep the subscription alive
//   auto token = EventBus::Instance().Subscribe(EventType::CharacterDamaged, myHandler);
//
//   // Unsubscribe explicitly:
//   token.reset();   // or just let it go out of scope
// ============================================================================

using SubscriptionToken = std::shared_ptr<bool>;

// ============================================================================
// EVENT BUS - Singleton Pattern
// ============================================================================
// Central hub for game events using the Observer Pattern.
//
// USAGE:
//   // Subscribe to events (store the returned token!)
//   auto token = EventBus::Instance().Subscribe(EventType::CharacterDamaged,
//       [](const GameEvent& e) {
//           std::cout << e.target->GetName() << " took " << e.value << " damage\n";
//       });
//
//   // Publish events (from Character, CombatEngine, etc.)
//   EventBus::Instance().Publish({EventType::CharacterDamaged, attacker, defender, damage});
//
//   // Unsubscribe when done (or let token go out of scope)
//   token.reset();
//
// ============================================================================

using EventHandler = std::function<void(const GameEvent&)>;

class EventBus {
public:
    // Singleton access - returns the single global instance
    static EventBus& Instance();
    
    // Subscribe to an event type. Returns a token that controls the
    // subscription's lifetime. Hold the token to stay subscribed.
    [[nodiscard("Discarding the token will cause the subscription to be cancelled")]]
    SubscriptionToken Subscribe(EventType type, EventHandler handler);
    
    void Publish(const GameEvent& event);
    
    void ClearAll();
    
    // Delete copy/move to enforce singleton
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;
    
private:
    EventBus() = default;  // Private constructor
    
    // Internal pairing of a handler with its lifetime token.
    // The EventBus holds a weak_ptr (non-owning observer).
    // The caller holds the shared_ptr (owner).
    // When the caller's shared_ptr is destroyed, the weak_ptr expires,
    // and the handler is cleaned up during the next Publish().
    struct Subscription {
        std::weak_ptr<bool> token;
        EventHandler handler;
    };
    
    // Map of event type -> list of subscriptions
    std::unordered_map<EventType, std::vector<Subscription>> m_Subscribers;
};
