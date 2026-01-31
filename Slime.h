#pragma once
#include "Monster.h"

class Slime : public Monster {
public:
    Slime(); 
    std::string GetClassName() override;
};
