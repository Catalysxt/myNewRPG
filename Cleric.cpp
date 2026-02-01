#include <iostream>
#include "Cleric.h"
#include "CombatEngine.h"

// Cleric: HP 140, STR 4, INT 6, AGI 2, ARM 3, RES 4
// Balanced stats, uses Magical damage
Cleric::Cleric(std::string name)
    : Character(name, 140, StatBlock(4, 6, 2, 3, 4)) {
    m_DamageStrategy = std::make_unique<MagicalDamageStrategy>();
}

std::string Cleric::GetClassName() {
    return "Cleric";
}

void Cleric::LevelUp() {
    Character::LevelUp();
    m_Stats.IncreaseStats(1, 1, 0, 1, 2); // +1 STR, +1 INT, +1 ARM, +2 RES
    std::cout << "Cleric Specialization: Bonus +2 Resistance!" << std::endl;
}

// ============================================================================
// DIVINE HEAL - Cleric's Special Ability
// ============================================================================
// Channel holy power to heal yourself.
// - Heals for 65 + Intellect HP
// - Cures BURNING and POISONED status effects
// - NOTE: This heals SELF, ignores the target parameter
// ============================================================================

void Cleric::ExecuteSpecialAbility(Character* /*target*/, CombatEngine& /*engine*/) {
    std::cout << m_Name << " channels divine energy!" << std::endl;
    
    // Calculate heal amount: base 65 + INT bonus
    int healAmount = 65 + m_Stats.GetIntellect();
    
    // Heal self
    Heal(healAmount);
    
    // Cure debuffs
    bool curedSomething = false;
    
    if (HasStatus(StatusEffect::Burning)) {
        RemoveStatus(StatusEffect::Burning);
        curedSomething = true;
    }
    if (HasStatus(StatusEffect::Poisoned)) {
        RemoveStatus(StatusEffect::Poisoned);
        curedSomething = true;
    }
    
    if (curedSomething) {
        std::cout << m_Name << "'s ailments are cleansed!" << std::endl;
    }
}
