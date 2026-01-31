#include "Slime.h"

// Slime Stats:
// HP: 40
// STR: 2, INT: 1, AGI: 2, ARM: 0, RES: 2
Slime::Slime()
    : Monster("Slime", 40, StatBlock(2, 1, 2, 0, 2)) {
}

std::string Slime::GetClassName() {
    return "Slime";
}
