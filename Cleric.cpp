#include <iostream>
#include "Cleric.h"

// Cleric uses MAGICAL damage - divine smite scales with Intellect
// Balanced stats but magic-focused attacks
Cleric::Cleric(std::string name)
    : Character(name, 140, StatBlock(4, 6, 2, 3, 4)) {
    m_DamageStrategy = std::make_unique<MagicalDamageStrategy>();
}

std::string Cleric::GetClassName() {
    return "Cleric";
}

void Cleric::LevelUp() {
    Character::LevelUp();
    // Specialized Growth: Balanced + RES
    m_Stats.IncreaseStats(1, 1, 0, 1, 2); // +1 STR, +1 INT, +1 ARM, +2 RES
    std::cout << "Cleric Specialization: Bonus +2 Resistance!" << std::endl;
}
