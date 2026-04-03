#include <iostream>
#include "SlimeBubble.h"
#include "Character.h"

SlimeBubble::SlimeBubble() 
    : Item("Slime Bubble", "A squishy bubble from a slime. Maybe useful for crafting?") {} // (name, description)

void SlimeBubble::Use(Character* user) {
    std::cout << user->GetName() << " examines the Slime Bubble..." << std::endl;
    std::cout << "It's squishy and jiggles. Maybe useful for crafting?" << std::endl;
}
