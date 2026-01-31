#pragma once
#include <string>
#include <vector>
#include <memory>
#include "StatBlock.h"

class Item; // Forward Declaration

class Character {
public:
    Character(std::string name, int hp, StatBlock stats);

    // Virtual Destructor
    virtual ~Character();

    void TakeDamage(int damage);
    void Heal(int amount);

    std::string GetName();
    
    // Abstract Method
    virtual std::string GetClassName() = 0;
    
    int GetCurrentHP();
    int GetMaxHP();
    int GetLevel();
    StatBlock GetStats();

    // Inventory System
    void AddItem(std::unique_ptr<Item> item);
    void UseItem(int index);
    bool HasItems();
    void PrintInventory();

    // Gold System
    // Justification: Encapsulation - Gold is a private member, accessed via getters/setters.
    int GetGold() const;
    void AddGold(int amount);
    bool SpendGold(int amount); // Returns false if insufficient funds

protected:
    // XP System
    virtual void LevelUp();
    
    std::string m_Name;
    int m_Level;
    int m_CurrentHP;
    int m_MaxHP;
    StatBlock m_Stats;

    int m_CurrentXP;
    int m_MaxXP;

    // Inventory
    std::vector<std::unique_ptr<Item>> m_Inventory;

    // Economy
    int m_Gold;

public:
    void GainXP(int amount);
};
