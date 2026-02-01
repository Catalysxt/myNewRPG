#include "Monster.h"
#include "DamageStrategy.h"

// Monsters default to PHYSICAL damage
// Specific monsters (like a Mage enemy) could override this
Monster::Monster(std::string name, int hp, StatBlock stats)
    : Character(name, hp, stats) {
    m_DamageStrategy = std::make_unique<PhysicalDamageStrategy>();
}

void Monster::LevelUp() {
    Character::LevelUp(); // Re-use LevelUp() from Character.cpp

    // Default monster growth: balanced small increase
    m_Stats.IncreaseStats(1, 1, 1, 1, 1);
    m_MaxHP += 10;
    m_CurrentHP = m_MaxHP;
}
