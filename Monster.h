#pragma once
#include "Character.h"

// Monster inherits from Character.
// We can add monster-specific logic here later (like GoldDrop or Aggro).
class Monster : public Character {
public:
    Monster(std::string name, int hp, StatBlock stats);

    // We implement LevelUp here so we don't HAVE to do it for every single monster type,
    // unless a specific monster has unique growth.
    void LevelUp() override;
};
