#pragma once
#include "StatBlock.h"

// ============================================================================
// STRATEGY PATTERN: Damage Calculation
// ============================================================================
// The Strategy Pattern defines a family of algorithms (damage formulas),
// encapsulates each one, and makes them interchangeable at runtime.
//
// WHY USE THIS?
// - Different attacks use different stats (Strength vs Intellect)
// - We can add new damage types (poison, true damage) without modifying existing code
// - Combat modifiers (buffs, debuffs) can wrap existing strategies (Decorator pattern later)
// ============================================================================

class DamageStrategy {
public:
    virtual ~DamageStrategy() = default;

    // Returns: Final calculated damage
    virtual int CalculateDamage(int baseDamage, const StatBlock& attackerStats) const = 0;

    virtual const char* GetName() const = 0;
};

// ============================================================================
// CONCRETE STRATEGY: Physical Damage
// ============================================================================
// Used by: Warriors, Rogues, basic attacks
// ============================================================================

class PhysicalDamageStrategy : public DamageStrategy {
public:
    int CalculateDamage(int baseDamage, const StatBlock& attackerStats) const override {
        return baseDamage + (attackerStats.GetStrength() / 2); // Formula
    }

    const char* GetName() const override { return "Physical"; }
};

// ============================================================================
// CONCRETE STRATEGY: Magical Damage
// ============================================================================
// Formula: baseDamage + (Intellect * 0.75)
// Used by: Wizards, Clerics, spell abilities
// ============================================================================

class MagicalDamageStrategy : public DamageStrategy {
public:
    int CalculateDamage(int baseDamage, const StatBlock& attackerStats) const override {
        return baseDamage + static_cast<int>(attackerStats.GetIntellect() * 1.2);
    }

    const char* GetName() const override { return "Magical"; }
};

// ============================================================================
// CONCRETE STRATEGY: True Damage (Ignores Armor)
// ============================================================================
// Formula: baseDamage (no modifiers, no reduction)
// Used by: Special abilities, poison, execution effects
// ============================================================================

class TrueDamageStrategy : public DamageStrategy {
public:
    int CalculateDamage(int baseDamage, const StatBlock& /*attackerStats*/) const override {
        // True damage is unmodified - it bypasses all calculations
        return baseDamage;
    }

    const char* GetName() const override { return "True"; }
};
