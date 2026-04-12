#pragma once
#include <random>
#include <memory>
#include <iostream>
#include "DamageStrategy.h"
#include "StatBlock.h"

// Forward declarations
class Character;
class Monster;

// ============================================================================
// COMBAT ENGINE
// ============================================================================
// The CombatEngine centralizes ALL combat calculations:
// - Damage calculation (using DamageStrategy)
// - Critical hit rolls
// - Dodge rolls
// - Armor mitigation
//
// WHY CENTRALIZE THIS?
// 1. Single source of truth for combat math
// 2. Encapsulates the random number generator (no global state)
// ============================================================================

// Result of an attack calculation - contains all the details
struct AttackResult {
    int rawDamage;       // Before mitigation
    int finalDamage;     // After armor/resistance
    bool isCritical;     // Was this a critical hit?
    bool wasDodged;      // Did the defender dodge?
    DamageType damageType;  // Physical, Magical, True, etc.
};

class CombatEngine {
public:

    CombatEngine();

    // Calculate a full attack from attacker to defender
    AttackResult CalculateAttack(
        const StatBlock& attackerStats,
        const StatBlock& defenderStats,
        const DamageStrategy& strategy,
        int baseDamage
    );

    // Convenience overload using default Physical damage
    AttackResult CalculatePhysicalAttack(
        const StatBlock& attackerStats,
        const StatBlock& defenderStats,
        int baseDamage
    );

    // ========================================================================
    // INDIVIDUAL ROLL METHODS (for special abilities that need direct access)
    // ========================================================================

    bool RollCritical(int critChance);

    bool RollDodge(int dodgeChance);

    // Get a random damage variance (+/- percentage)
    // Returns a multiplier, e.g., 0.9 to 1.1 for 10% variance
    float RollDamageVariance(float variancePercent = 0.1f);

    // ========================================================================
    // CONFIGURATION
    // ========================================================================
    
    // Critical hits multiply damage by this value (default: 2.0)
    void SetCritMultiplier(float multiplier) { m_CritMultiplier = multiplier; }
    float GetCritMultiplier() const { return m_CritMultiplier; }

    // Base critical chance before Agility bonus (default: 5%)
    void SetBaseCritChance(int chance) { m_BaseCritChance = chance; }
    int GetBaseCritChance() const { return m_BaseCritChance; }

    // Access to random engine (for other systems that need randomness)
    std::mt19937& GetRandomEngine() const { return m_RandomEngine; }

private:
    // ========================================================================
    // RANDOM NUMBER GENERATION - The Modern C++ Way
    // ========================================================================
    mutable std::mt19937 m_RandomEngine;  // Mersenne Twister pseudo-random generator

    // Pre-created distributions for common roll types
    // Creating distributions is cheap, but reusing them is cleaner
    std::uniform_int_distribution<int> m_PercentRoll{1, 100};
    std::uniform_real_distribution<float> m_VarianceRoll{0.9f, 1.1f};

    // ========================================================================
    // INTERNAL HELPERS
    // ========================================================================
    
    // Calculate how much armor/resistance reduces incoming damage
    int ApplyMitigation(int damage, const StatBlock& defenderStats, bool isMagical);

    // Calculate effective crit chance from Agility
    int CalculateCritChance(const StatBlock& attackerStats);

    // Calculate effective dodge chance from Agility
    int CalculateDodgeChance(const StatBlock& defenderStats);

    // Combat tuning parameters
    float m_CritMultiplier = 2.0f; // Critical hits double our damage
    int m_BaseCritChance = 5;  // 5% base crit chance
    int m_BaseDodgeChance = 2;  // 2% base dodge chance
};
