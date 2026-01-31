#include <iostream>
#include "Rogue.h"

// Rogue utilizes the Base Character constructor.
// HP: 120 (Medium-Low)
// STR: 6, INT: 2, AGI: 10, ARM: 2, RES: 1
Rogue::Rogue(std::string name)
    : Character(name, 120, StatBlock(6, 2, 10, 2, 1)) {
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
