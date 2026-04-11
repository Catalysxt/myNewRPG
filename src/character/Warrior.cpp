#include <iostream>
#include <string_view>

#include "Warrior.h"
#include "CombatEngine.h"

// Warrior: HP 200, STR 10, INT 2, AGI 4, ARM 5, RES 2
// Uses Physical damage strategy
Warrior::Warrior(std::string name)
    : Character(name, 200, StatBlock(10, 2, 4, 5, 2)) {
    m_DamageStrategy = std::make_unique<PhysicalDamageStrategy>();
}

std::string_view Warrior::GetClassName() {
    return "Warrior";
}

void Warrior::LevelUp() {

    Character::LevelUp(); 

    // Specialized Growth: Warriors gain LOTS of STR and HP.
    m_Stats.IncreaseStats(3, 0, 1, 1, 0); // +3 STR, +1 AGI, +1 ARM
    m_MaxHP += 20; // Bonus HP growth
    m_CurrentHP = m_MaxHP; // Heal the bonus amount

    std::cout << "Warrior Specialization: Bonus +20 HP and +3 Strength!" << std::endl;
}

// ============================================================================
// SHIELD BASH - Warrior's Special Ability
// ============================================================================
// Deals 75% of normal damage but has a 40% chance to STUN the target.
// Stunned targets skip their next turn.
//
// DATA FLOW:
//   1. Warrior calls PerformSpecialAbility() (template method in Character)
//   2. Template method checks RP, deducts cost, calls ExecuteSpecialAbility()
//   3. ExecuteSpecialAbility() uses CombatEngine for damage + stun roll
//   4. If stun succeeds, target->ApplyStatus(StatusEffect::Stunned)
//   5. main.cpp checks target->HasStatus(Stunned) at start of turn
// ============================================================================

void Warrior::ExecuteSpecialAbility(Character* target, CombatEngine& engine) {
    std::cout << m_Name << " raises shield and charges!" << std::endl;
    
    // Calculate reduced damage (75% of normal attack)
    int baseDamage = static_cast<int>(GetBaseDamage() * 0.75);
    
    AttackResult result = engine.CalculateAttack(
        m_Stats,
        target->GetStats(),
        GetDamageStrategy(),
        baseDamage
    );
    
    if (result.wasDodged) {
        std::cout << "SHIELD BASH missed!" << std::endl;
        return;
    }
    
    // Apply damage
    std::cout << "SHIELD BASH hits for " << result.finalDamage << " damage!" << std::endl;
    target->TakeDamage(result.finalDamage);
    
    const int STUN_CHANCE = 40; // 40% stun chance
    if (engine.RollCritical(STUN_CHANCE)) {  // Reusing crit roll as generic % check
        target->ApplyStatus(StatusEffect::Stunned);
        std::cout << target->GetName() << " is STUNNED! They will skip their next turn!" << std::endl;
    }
}
