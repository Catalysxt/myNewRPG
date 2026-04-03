#pragma once
#include "Item.h"

class Potion : public Item {
public:
    Potion(int healAmount);
    void Use(Character* user) override;
    bool IsConsumable() override { return true; }

private:
    int m_HealAmount;
};
