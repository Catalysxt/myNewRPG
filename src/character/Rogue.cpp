#include <iostream>
#include <string_view>

#include "Rogue.h"
#include "CombatEngine.h"

// Rogue: HP 120, STR 6, INT 2, AGI 10, ARM 2, RES 1
// High Agility = bonus crit/dodge chances
Rogue::Rogue(std::string name)
    : Character(name, 120, StatBlock(6, 2, 10, 2, 1)) {
    m_DamageStrategy = std::make_unique<PhysicalDamageStrategy>();
}

std::string_view Rogue::GetClassName() {
    return "Rogue";
}

void Rogue::LevelUp() {
    Character::LevelUp();
    m_Stats.IncreaseStats(1, 0, 3, 1, 0); // +1 STR, +3 AGI, +1 ARM
    std::cout << "Rogue Specialization: Bonus +3 Agility!" << std::endl;
}

// ============================================================================
// BACKSTAB - Rogue's Special Ability
// ============================================================================
// Strike from the shadows with deadly precision.
// - Deals 150% of normal damage
// - ALWAYS a critical hit (2x damage on top of the 150%)
// - Total multiplier: 1.5 * 2.0 = 3x damage!
// ============================================================================

void Rogue::ExecuteSpecialAbility(Character* target, CombatEngine& engine) {
    std::cout << m_Name << " vanishes into the shadows..." << std::endl;
    
    // Calculate boosted damage (150% of base)
    int baseDamage = static_cast<int>(GetBaseDamage() * 1.5);
    
    // Calculate attack (we'll force critical afterward)
    AttackResult result = engine.CalculateAttack(
        m_Stats,
        target->GetStats(),
        GetDamageStrategy(),
        baseDamage
    );
    
    if (result.wasDodged) {
        std::cout << "BACKSTAB missed! The target sensed danger!" << std::endl;
        return;
    }
    
    // Force critical damage (apply crit multiplier if not already crit)
    if (!result.isCritical) {
        result.finalDamage = static_cast<int>(result.finalDamage * engine.GetCritMultiplier());
        result.isCritical = true;
    }
    
    std::cout << "BACKSTAB! Critical strike for " << result.finalDamage << " damage!" << std::endl;
    target->TakeDamage(result.finalDamage);
}
