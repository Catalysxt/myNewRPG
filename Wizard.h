#pragma once
#include "Character.h"

class Wizard : public Character {
public:
    Wizard(std::string name);
    std::string GetClassName() override;
    void LevelUp() override;
};
