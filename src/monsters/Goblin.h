#pragma once
#include "Monster.h"

// Goblin - Fast and dodgy, but weak
// 10% spawn chance
class Goblin : public Monster {
public:
    Goblin();
    std::string GetClassName() override;
};
