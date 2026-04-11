#pragma once
#include <string_view>

#include "Character.h"

// Rogue - Agility-based class with high crit/dodge
// Special Ability: BACKSTAB - Guaranteed critical hit
class Rogue : public Character {
public:
    Rogue(std::string name);
    std::string_view GetClassName() override;
    std::string GetAbilityName() const override { return "Backstab"; }
    void LevelUp() override;

protected:
    void ExecuteSpecialAbility(Character* target, CombatEngine& engine) override;
    int GetAbilityCost() const override { return 60; }
};
