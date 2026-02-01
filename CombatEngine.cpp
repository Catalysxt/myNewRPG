#include "CombatEngine.h"
#include <algorithm> // For std::max, std::clamp

// ============================================================================
// CONSTRUCTOR
// ============================================================================
// The random engine is seeded ONCE here using std::random_device, which
// provides hardware-based entropy (on most systems). This is the correct
// way to seed a PRNG in modern C++.
//
// IMPORTANT: std::random_device{} creates a temporary object and calls 
// its operator() to get a random seed value. The {} syntax is C++11
// uniform initialization.
// ============================================================================

CombatEngine::CombatEngine()
    : m_RandomEngine(std::random_device{}())  // Seed from hardware entropy
{
    // All other members use in-class initialization (C++11 feature)
    // See the declarations in CombatEngine.h
}

// ============================================================================
// MAIN ATTACK CALCULATION
// ============================================================================
// This is the heart of the combat system. The flow is:
// 1. Check for dodge (defender's Agility)
// 2. Calculate base damage using the Strategy pattern
// 3. Apply damage variance (±10% randomness)
// 4. Check for critical hit (attacker's Agility)
// 5. Apply armor/resistance mitigation
// 6. Return the complete AttackResult
// ============================================================================

AttackResult CombatEngine::CalculateAttack(
    const StatBlock& attackerStats,
    const StatBlock& defenderStats,
    const DamageStrategy& strategy,
    int baseDamage)
{
    AttackResult result;
    result.damageType = strategy.GetName();
    result.wasDodged = false;
    result.isCritical = false;

    // --- Step 1: Dodge Check ---
    int dodgeChance = CalculateDodgeChance(defenderStats);
    if (RollDodge(dodgeChance)) {
        result.wasDodged = true; // Enemy successfully dodged the attack
        result.rawDamage = 0;
        result.finalDamage = 0;
        return result;  // Early exit - attack missed entirely
    }

    // --- Step 2: Calculate Raw Damage via Strategy ---
    // The Strategy pattern allows different formulas without if/else chains
    int damage = strategy.CalculateDamage(baseDamage, attackerStats);

    // --- Step 3: Apply Damage Variance ---
    // Makes combat feel less "mechanical" - damage varies by ±10%
    float variance = RollDamageVariance();
    damage = static_cast<int>(damage * variance);

    // --- Step 4: Critical Hit Check ---
    int critChance = CalculateCritChance(attackerStats);
    if (RollCritical(critChance)) {
        result.isCritical = true; // We landed a critical hit
        damage = static_cast<int>(damage * m_CritMultiplier);
    }

    result.rawDamage = damage;

    // --- Step 5: Apply Mitigation (Armor or Resistance) ---
    // Magical damage is reduced by Resistance, Physical by Armor
    bool isMagical = (strategy.GetName() == std::string("Magical"));
    result.finalDamage = ApplyMitigation(damage, defenderStats, isMagical);

    // Ensure we always deal at least 1 damage (unless dodged)
    result.finalDamage = std::max(1, result.finalDamage);

    return result;
}

// ============================================================================
// CONVENIENCE OVERLOAD: Physical Attack
// ============================================================================
// Most attacks in the game are physical, so we provide a shortcut that
// doesn't require creating a strategy object every time.
// ============================================================================

AttackResult CombatEngine::CalculatePhysicalAttack(
    const StatBlock& attackerStats,
    const StatBlock& defenderStats,
    int baseDamage)
{
    // Static local instance - created once, reused forever
    // This is thread-safe in C++11+ (magic statics)
    static PhysicalDamageStrategy physicalStrategy;
    return CalculateAttack(attackerStats, defenderStats, physicalStrategy, baseDamage);
}

// ============================================================================
// ROLL METHODS
// ============================================================================

bool CombatEngine::RollCritical(int critChance) {
    // Clamp to valid range [0, 100]
    critChance = std::clamp(critChance, 0, 100);
    
    // Roll 1-100, if roll <= critChance, it's a crit
    // Example: critChance=15 means rolls 1-15 are crits (15% chance)
    return m_PercentRoll(m_RandomEngine) <= critChance;
}

bool CombatEngine::RollDodge(int dodgeChance) {
    dodgeChance = std::clamp(dodgeChance, 0, 100);
    return m_PercentRoll(m_RandomEngine) <= dodgeChance;
}

float CombatEngine::RollDamageVariance(float variancePercent) {
    // Default variance is ±10%, so we roll between 0.9 and 1.1
    // We recreate the distribution here because variance might change
    std::uniform_real_distribution<float> dist(
        1.0f - variancePercent, 
        1.0f + variancePercent
    );
    return dist(m_RandomEngine);
}

// ============================================================================
// MITIGATION CALCULATION
// ============================================================================
// Armor and Resistance reduce incoming damage using a diminishing returns
// formula. This prevents high armor from making characters invincible.
//
// Formula: reduction = armor / (armor + 100)
// Examples:
//   - 0 armor   → 0% reduction
//   - 50 armor  → 33% reduction  
//   - 100 armor → 50% reduction
//   - 200 armor → 66% reduction
// ============================================================================

int CombatEngine::ApplyMitigation(int damage, const StatBlock& defenderStats, bool isMagical) {
    int mitigation = isMagical 
        ? defenderStats.GetElementRes() 
        : defenderStats.GetArmor();

    // Diminishing returns formula
    float reductionPercent = static_cast<float>(mitigation) / (mitigation + 100.0f);

    // Apply reduction
    int reducedDamage = static_cast<int>(damage * (1.0f - reductionPercent));

    return reducedDamage;
}

// ============================================================================
// CRIT/DODGE CHANCE CALCULATIONS
// ============================================================================
// Agility contributes to both crit and dodge chances.
// Each point of Agility adds 0.5% to crit and 0.3% to dodge.
// ============================================================================

int CombatEngine::CalculateCritChance(const StatBlock& attackerStats) {
    // Base crit + Agility bonus (2 AGI = 1% crit)
    int critFromAgility = attackerStats.GetAgility() / 2;
    return m_BaseCritChance + critFromAgility;
}

int CombatEngine::CalculateDodgeChance(const StatBlock& defenderStats) {
    
    return m_BaseDodgeChance +defenderStats.GetAgility() / 3;
    // (3 AGI = 1% dodge)
}
