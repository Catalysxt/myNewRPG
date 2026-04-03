#pragma once
#include "Item.h"

// Slime Bubble - Crafting material dropped by Slimes
class SlimeBubble : public Item {
public:
    SlimeBubble();
    
    void Use(Character* user) override;
    bool IsConsumable() override { return false; } // Not a consumable 
};
