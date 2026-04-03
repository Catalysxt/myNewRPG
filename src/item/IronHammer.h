#pragma once
#include "Item.h"

// Iron Hammer - Weapon dropped by Orcs
class IronHammer : public Item {
public:
    IronHammer();
    
    void Use(Character* user) override;
    bool IsConsumable() override { return false; }
};
