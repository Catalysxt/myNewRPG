#pragma once
#include "Monster.h"

// Slime - Weak starter enemy
// Drops: Slime Bubble (25% chance)
class Slime : public Monster {
public:
    Slime(); 
    std::string GetClassName() override;
    
    // 25% chance to drop Slime Bubble
    std::unique_ptr<Item> GetLootDrop(CombatEngine& engine) const override;
};
