#pragma once
#include <string_view>

#include "Character.h"

// Cleric - Support class with balanced stats
// Special Ability: DIVINE HEAL - Heals self and cures status effects
class Cleric : public Character {
public:
    Cleric(std::string name);
    std::string_view GetClassName() override;
    std::string GetAbilityName() const override { return "Divine Heal"; }
    void LevelUp() override;

protected:
    void ExecuteSpecialAbility(Character* target, CombatEngine& engine) override;
    int GetAbilityCost() const override { return 55; }
};
