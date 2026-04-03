#pragma once
#include <string>

// Forward declaration
class Character;

// ============================================================================
// GAME EVENT TYPES
// ============================================================================
// All events that can be published through the EventBus.
// Add new event types here as needed.
// ============================================================================

enum class EventType {
    CharacterDamaged,   // When any character takes damage
    CharacterHealed,    // When any character is healed
    CharacterDied,      // When HP reaches 0
    CharacterLeveledUp, // When a character levels up
    GoldGained,         // When gold is acquired
    ItemPickedUp,       // When an item is added to inventory
    BattleStarted,      // When combat begins
    BattleEnded,        // When combat ends (victory or defeat)
    MaxRP               // When user has maximum RP
};

// A uniform structure for all events. Not all fields are used by all events.

struct GameEvent {
    EventType type;
    Character* source = nullptr;  // Who caused the event (e.g., attacker)
    Character* target = nullptr;  // Who received the event (e.g., defender)
    int value = 0;                // Context-specific (damage, gold, XP, etc.)
    std::string message;          // Optional description
};
