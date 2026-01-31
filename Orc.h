#pragma once
#include "Monster.h"

class Orc : public Monster {
public:
    Orc(); // Default constructor: "Orc"
    std::string GetClassName() override;
};
