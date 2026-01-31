#include <iostream>
#include "Wizard.h"

// Wizard utilizes the Base Character constructor.
// HP: 100 (Low)
// STR: 1, INT: 10, AGI: 3, ARM: 1, RES: 5
Wizard::Wizard(std::string name)
    : Character(name, 100, StatBlock(1, 10, 3, 1, 5)) {
}

std::string Wizard::GetClassName() {
    return "Wizard";
}

void Wizard::LevelUp() {
    Character::LevelUp();
    // Specialized Growth: Wizards gain LOTS of INT and RES.
    m_Stats.IncreaseStats(0, 3, 1, 0, 1); // +3 INT, +1 AGI, +1 RES
    std::cout << "Wizard Specialization: Bonus +3 Intellect!" << std::endl;
}
