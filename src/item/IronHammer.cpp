#include <iostream>
#include "IronHammer.h"
#include "Character.h"

IronHammer::IronHammer() 
    : Item("Iron Hammer", "A sturdy weapon. +5 damage when equipped.") {}

void IronHammer::Use(Character* user) {
    std::cout << user->GetName() << " swings the Iron Hammer menacingly!\n" ;
    std::cout << "+5 STR added\n";

    user->IncreaseStr(5);
}
