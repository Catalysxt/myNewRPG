#include <string_view>

#include "Orc.h"
#include "CombatEngine.h"
#include "IronHammer.h"

// Orc Stats: HP 30, STR 8, INT 1, AGI 2, ARM 3, RES 1
Orc::Orc()
    : Monster("Orc", 30, StatBlock(8, 1, 2, 3, 1)) {
    m_MinGold = 10;
    m_MaxGold = 25;
    m_LootDropChance = 15;  // 15% chance for Iron Hammer
}

std::string_view Orc::GetClassName() {
    return "Orc";
}

std::unique_ptr<Item> Orc::GetLootDrop(CombatEngine& engine) const {
    std::uniform_int_distribution<int> dist(1, 100);
    if (dist(engine.GetRandomEngine()) <= m_LootDropChance) {
        return std::make_unique<IronHammer>();
    }
    return nullptr;
}
