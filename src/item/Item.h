#pragma once
#include <string>
#include <iostream>

// Forward Declaration:
class Character;

class Item {
public:
    Item(std::string name, std::string description);
    virtual ~Item() {}

    std::string GetName();
    std::string GetDescription();

    // Pure Virtual Function:
    // Every item must define what happens when it is "Used".
    virtual void Use(Character* user) = 0;

    // Default: Items are not consumed (like Swords). Override for Potions.
    virtual bool IsConsumable() { return false; }

protected:
    std::string m_Name;
    std::string m_Description;
};
