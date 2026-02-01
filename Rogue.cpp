#include <iostream>
#include "Rogue.h"

// Rogue uses PHYSICAL damage - daggers and swords scale with Strength
// Their high Agility gives crit/dodge bonuses separately
Rogue::Rogue(std::string name)
    : Character(name, 120, StatBlock(6, 2, 10, 2, 1)) {
    m_DamageStrategy = std::make_unique<PhysicalDamageStrategy>();
}

std::string Rogue::GetClassName() {
    return "Rogue";
}

void Rogue::LevelUp() {
    Character::LevelUp();
    // Specialized Growth: Agility
    m_Stats.IncreaseStats(1, 0, 3, 1, 0); // +1 STR, +3 AGI, +1 ARM
    std::cout << "Rogue Specialization: Bonus +3 Agility!" << std::endl;
}
