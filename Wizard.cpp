#include <iostream>
#include "Wizard.h"

// Wizard uses MAGICAL damage - their attacks scale with Intellect
Wizard::Wizard(std::string name)
    : Character(name, 100, StatBlock(1, 10, 3, 1, 5)) {
    m_DamageStrategy = std::make_unique<MagicalDamageStrategy>();
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
