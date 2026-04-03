#include "Slime.h"
#include "CombatEngine.h"
#include "SlimeBubble.h"

// Slime Stats: HP 40, STR 2, INT 1, AGI 2, ARM 0, RES 2
Slime::Slime()
    : Monster("Slime", 40, StatBlock(2, 1, 2, 0, 2)) {
    m_MinGold = 3;
    m_MaxGold = 8;
    m_LootDropChance = 25;  // 25% chance for Slime Bubble
}

std::string Slime::GetClassName() {
    return "Slime";
}

std::unique_ptr<Item> Slime::GetLootDrop(CombatEngine& engine) const {
    // Roll for loot drop
    std::uniform_int_distribution<int> dist(1, 100);
    if (dist(engine.GetRandomEngine()) <= m_LootDropChance) {
        return std::make_unique<SlimeBubble>();
    }
    return nullptr;
}
