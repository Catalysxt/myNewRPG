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
    
    // Getters 
    int GetCurrentHP() const;
    int GetMaxHP() const;
    int GetLevel() const;
    StatBlock GetStats() const;
    
    // Base damage for unarmed attacks (can be overridden for weapon-based damage)
    virtual int GetBaseDamage() const;

    // Inventory System
    void AddItem(std::unique_ptr<Item> item);
    void UseItem(int index);
    bool HasItems();
    void PrintInventory();

    // Gold System
    int GetGold() const;
    void AddGold(int amount);
    bool SpendGold(int amount); // Returns false if insufficient funds

    // XP System (public so battles can reward XP)
    void GainXP(int amount);

protected:
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
};
