#pragma once
#include <string>
#include <vector>
#include <memory>
#include "StatBlock.h"
#include "DamageStrategy.h"
#include "StatusEffect.h"

class Item; // Forward Declaration
class CombatEngine; // Forward Declaration for special abilities

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
    
    // Damage strategy (Physical, Magical, etc.)
    const DamageStrategy& GetDamageStrategy() const;
    
    // =========================================================================
    // RESOURCE POINT SYSTEM
    // =========================================================================
    // Resource Points (RP) are used for special abilities.
    // Different classes interpret RP differently (Mana, Stamina, Rage)
    int GetCurrentRP() const;
    int GetMaxRP() const;
    void SpendRP(int amount);
    void RestoreRP(int amount); // If we level up, RP is restored to max
    
    // =========================================================================
    // STATUS EFFECT SYSTEM
    // =========================================================================
    void ApplyStatus(StatusEffect effect);
    void RemoveStatus(StatusEffect effect);
    bool HasStatus(StatusEffect effect) const;
    void ClearAllStatus();
    StatusEffect GetCurrentStatus() const;
    
    // =========================================================================
    // SPECIAL ABILITY SYSTEM (Template Method Pattern)
    // =========================================================================
    // PerformSpecialAbility is the TEMPLATE METHOD - it defines the skeleton:
    //   1. Check if can use ability (enough RP?)
    //   2. Consume RP
    //   3. Execute the ability (subclass hook)
    //
    // Subclasses override ExecuteSpecialAbility() to define unique behavior.
    void PerformSpecialAbility(Character* target, CombatEngine& engine);
    
    // Get the name of this class's special ability (for UI display)
    virtual std::string GetAbilityName() const { return "Basic Attack"; }
    
    // How much RP does this ability cost? Override per class.
    virtual int GetAbilityCost() const { return 40; }

protected:
    // Hook method - each class implements their unique ability here
    virtual void ExecuteSpecialAbility(Character* target, CombatEngine& engine);

public:
    // Inventory System
    void AddItem(std::unique_ptr<Item> item);
    void UseItem(int index);
    bool HasItems();
    void PrintInventory();

    // Gold System
    int GetGold() const;
    void AddGold(int amount);
    bool SpendGold(int amount); // Returns false if insufficient funds

    // XP System 
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
    
    // Combat Strategy
    std::unique_ptr<DamageStrategy> m_DamageStrategy;
    
    // Resource Points (current value - max comes from StatBlock)
    int m_CurrentRP;
    
    // Status Effects (bit flags)
    StatusEffect m_Status;
};
