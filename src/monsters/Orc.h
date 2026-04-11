#pragma once
#include "Monster.h"

class Orc : public Monster {
public:
    Orc();
    std::string GetClassName() override;
    
    // 15% chance to drop Iron Hammer
    std::unique_ptr<Item> GetLootDrop(CombatEngine& engine) const override;
};
