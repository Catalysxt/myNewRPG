#pragma once
#include "Character.h"

// Warrior inherits from Character.
// All public/protected members of Character become part of Warrior.
class Warrior : public Character {
public:
    // We only need the name, because HP and Stats are "hardcoded" for the class archetypes.
    Warrior(std::string name);
    
    // Override the pure virtual function
    std::string GetClassName() override;
    
    // Customized Level Up
    void LevelUp() override;
};
