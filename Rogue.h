#pragma once
#include "Character.h"

class Rogue : public Character {
public:
    Rogue(std::string name);
    std::string GetClassName() override;
    void LevelUp() override;
};
