#include <iostream>
#include "Monster.h"
#include "DamageStrategy.h"
#include "CombatEngine.h"
#include "Item.h"

Monster::Monster(std::string name, int hp, StatBlock stats)
    : Character(name, hp, stats) {
    m_DamageStrategy = std::make_unique<PhysicalDamageStrategy>();
}

void Monster::LevelUp() {
    Character::LevelUp();
    m_Stats.IncreaseStats(1, 1, 1, 1, 1);
    m_MaxHP += 10;
    m_CurrentHP = m_MaxHP;
}

// ============================================================================
// LOOT SYSTEM IMPLEMENTATION
// ============================================================================

int Monster::GetGoldDrop(CombatEngine& engine) const {
    // Random gold between m_MinGold and m_MaxGold
    std::uniform_int_distribution<int> dist(m_MinGold, m_MaxGold);
    return dist(engine.GetRandomEngine());
}

std::unique_ptr<Item> Monster::GetLootDrop(CombatEngine& /*engine*/) const {
    // Base Monster class returns no loot - subclasses override this
    return nullptr;
}
