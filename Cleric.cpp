#include <iostream>
#include "Cleric.h"

// Cleric utilizes the Base Character constructor.
// HP: 140 (Medium)
// STR: 4, INT: 6, AGI: 2, ARM: 3, RES: 4
Cleric::Cleric(std::string name)
    : Character(name, 140, StatBlock(4, 6, 2, 3, 4)) {
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
