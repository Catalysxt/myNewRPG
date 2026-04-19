#pragma once
#include <cstdint>

// ============================================================================
// STATUS EFFECTS 
// ============================================================================
// Status effects are temporary conditions applied to characters during combat.
// They can be positive (buffs) or negative (debuffs).
//
// DESIGN: We use an enum class with bit shifting for efficiency:
// - Multiple effects can be combined: Stunned | Burning
// - Easy to check: if (status & StatusEffect::Stunned)
// - Easy to add: status |= StatusEffect::Stunned
// - Easy to remove: status &= ~StatusEffect::Stunned
// ============================================================================

// Each bit location represents a different status effect

enum class StatusEffect : uint8_t {
    None     = 0,
    Stunned  = 1 << 0,  // 0b00000001 - Skip next turn
    Burning  = 1 << 1,  // 0b00000010 - Take damage over time (future)
    Poisoned = 1 << 2,  // 0b00000100 - Take damage over time (future)
    Slowed   = 1 << 3,  // 0b00001000 - Reduced dodge chance (future)
    Blessed  = 1 << 4,  // 0b00010000 - Increased crit chance (future)
};

// ============================================================================
// BITWISE OPERATORS FOR StatusEffect via operator overloading
// ============================================================================
// These allow us to combine and check multiple status effects.
// Without these, using | or & on enum class would be a compile error.
// ============================================================================

constexpr StatusEffect operator|(StatusEffect a, StatusEffect b) {
    return static_cast<StatusEffect>(
        static_cast<uint8_t>(a) | static_cast<uint8_t>(b)
    );
}

constexpr StatusEffect operator&(StatusEffect a, StatusEffect b) {
    return static_cast<StatusEffect>(
        static_cast<uint8_t>(a) & static_cast<uint8_t>(b)
    );
}

constexpr StatusEffect operator~(StatusEffect a) {
    return static_cast<StatusEffect>(~static_cast<uint8_t>(a));
}

constexpr StatusEffect& operator|=(StatusEffect& a, StatusEffect b) {
    a = a | b;
    return a;
}

constexpr StatusEffect& operator&=(StatusEffect& a, StatusEffect b) {
    a = a & b;
    return a;
}

// Helper to check if a specific effect is active
constexpr bool HasEffect(StatusEffect status, StatusEffect effect) {
    return (status & effect) != StatusEffect::None;
}
