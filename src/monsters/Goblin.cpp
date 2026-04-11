#include <string_view>

#include "Goblin.h"

// Goblin Stats:
// HP: 25, STR: 3, INT: 2, AGI: 8, ARM: 1, RES: 1
// High agility = hard to hit, but low damage and HP
Goblin::Goblin()
    : Monster("Goblin", 25, StatBlock(3, 2, 8, 1, 1)) {
}

std::string_view Goblin::GetClassName() {
    return "Goblin";
}
