#pragma once
#include "Character.h"

class Cleric : public Character {
public:
    Cleric(std::string name);
    std::string GetClassName() override;
    void LevelUp() override;
};
