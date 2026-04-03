#pragma once
#include "Character.h"

// Warrior - Tank class with high HP and Strength
// Special Ability: SHIELD BASH - Deals damage and has a chance to STUN
class Warrior : public Character {
public:
    Warrior(std::string name);
    
    std::string GetClassName() override;
    std::string GetAbilityName() const override { return "Shield Bash"; }
    void LevelUp() override;

protected:
    // Warrior's unique ability: Shield Bash
    // Deals reduced damage but can STUN the target for 1 turn
    void ExecuteSpecialAbility(Character* target, CombatEngine& engine) override;
    
    int GetAbilityCost() const override { return 40; }
};
