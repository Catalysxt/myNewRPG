#include <iostream>
#include "Warrior.h"

// Warrior utilizes the Base Character constructor to set stats.
// HP: 200 (Highest)
// STR: 10, INT: 2, AGI: 4, ARM: 5, RES: 2
// EXPLANATION OF SYNTAX below:
// Warrior::Warrior(...)    -> This defines the constructor for the Warrior class.
// : Character(...)         -> This is the Member Initializer List. It calls the PARENT (Character) constructor.
// We MUST do this because a Warrior 'IS-A' Character, so the Character part needs to be built first.
Warrior::Warrior(std::string name)
    : Character(name, 200, StatBlock(10, 2, 4, 5, 2)) {
}

std::string Warrior::GetClassName() {
    return "Warrior";
}

void Warrior::LevelUp() {
    // Call Base Class LevelUp to handle XP reset, Level++, and HP heal.
    Character::LevelUp(); 

    // Specialized Growth: Warriors gain LOTS of STR and HP.
    m_Stats.IncreaseStats(3, 0, 1, 1, 0); // +3 STR, +1 AGI, +1 ARM
    m_MaxHP += 20; // Bonus HP growth
    m_CurrentHP = m_MaxHP; // Heal the bonus amount

    std::cout << "Warrior Specialization: Bonus +20 HP and +3 Strength!" << std::endl;
}
