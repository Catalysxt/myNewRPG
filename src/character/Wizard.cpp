#include <iostream>
#include <string_view>

#include "Wizard.h"
#include "CombatEngine.h"

// Wizard: HP 100, STR 1, INT 10, AGI 3, ARM 1, RES 5
// Magical damage scales with INT (1.2x multiplier)
Wizard::Wizard(std::string name)
    : Character(name, 100, StatBlock(1, 10, 3, 1, 5)) {
    m_DamageStrategy = std::make_unique<MagicalDamageStrategy>();
}

std::string_view Wizard::GetClassName() {
    return "Wizard";
}

void Wizard::LevelUp() {
    Character::LevelUp();
    m_Stats.IncreaseStats(0, 3, 1, 0, 1); // +3 INT, +1 AGI, +1 RES
    std::cout << "Wizard Specialization: Bonus +3 Intellect!" << std::endl;
}

// ============================================================================
// FIREBALL - Wizard's Special Ability
// ============================================================================
// Hurl a ball of fire at the enemy.
// - Deals 200% of base damage (magic scales with INT)
// - 25% chance to inflict BURNING status
// - Ignores armor (uses Element Resistance instead)
// ============================================================================

void Wizard::ExecuteSpecialAbility(Character* target, CombatEngine& engine) {
    std::cout << m_Name << " conjures a blazing FIREBALL!" << std::endl;
    
    // Double base damage for the spell
    int baseDamage = GetBaseDamage() * 2;
    
    AttackResult result = engine.CalculateAttack(
        m_Stats,
        target->GetStats(),
        GetDamageStrategy(),  // MagicalDamageStrategy (mitigated by RES, not ARM)
        baseDamage
    );
    
    if (result.wasDodged) {
        std::cout << "FIREBALL fizzles! The target evaded!" << std::endl;
        return;
    }
    
    std::cout << "FIREBALL hits for " << result.finalDamage << " fire damage!" << std::endl;
    target->TakeDamage(result.finalDamage);
    
    // 25% chance to inflict Burning
    const int BURN_CHANCE = 25;
    if (engine.RollCritical(BURN_CHANCE)) {
        target->ApplyStatus(StatusEffect::Burning);
        std::cout << target->GetName() << " is BURNING!" << std::endl; // Functionality hasn't been implemented 
    }
}
