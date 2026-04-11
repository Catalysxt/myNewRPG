#pragma once
#include <string_view>

#include "Character.h"

// Wizard - Intelligence-based spellcaster
// Special Ability: FIREBALL - High magic damage, chance to Burn
class Wizard : public Character {
public:
    Wizard(std::string name);
    std::string_view GetClassName() override;
    std::string GetAbilityName() const override { return "Fireball"; }
    void LevelUp() override;

protected:
    void ExecuteSpecialAbility(Character* target, CombatEngine& engine) override;
    int GetAbilityCost() const override { return 50; }
};
