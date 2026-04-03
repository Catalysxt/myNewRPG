#pragma once
#include <vector>
#include <unordered_map>
#include <functional>
#include "GameEvent.h"

// ============================================================================
// EVENT BUS - Singleton Pattern
// ============================================================================
// Central hub for game events using the Observer Pattern.
//
// USAGE:
//   // Subscribe to events
//   EventBus::Instance().Subscribe(EventType::CharacterDamaged, [](const GameEvent& e) {
//       std::cout << e.target->GetName() << " took " << e.value << " damage\n";
//   });
//
//   // Publish events (from Character, CombatEngine, etc.)
//   EventBus::Instance().Publish({EventType::CharacterDamaged, attacker, defender, damage});
//
// ============================================================================

using EventHandler = std::function<void(const GameEvent&)>;

class EventBus {
public:
    // Singleton access - returns the single global instance
    static EventBus& Instance();
    
    void Subscribe(EventType type, EventHandler handler);
    
    void Publish(const GameEvent& event);
    
    void ClearAll();
    
    // Delete copy/move to enforce singleton
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;
    
private:
    EventBus() = default;  // Private constructor
    
    // Map of event type -> list of handler functions
    std::unordered_map<EventType, std::vector<EventHandler>> m_Subscribers;
};
