#include "Orc.h"

// Orc Stats:
// HP: 100
// STR: 8, INT: 1, AGI: 2, ARM: 3, RES: 1
Orc::Orc()
    : Monster("Orc", 100, StatBlock(8, 1, 2, 3, 1)) {
}

std::string Orc::GetClassName() {
    return "Orc";
}
