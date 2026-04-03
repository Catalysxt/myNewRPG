#pragma once
#include <memory>
#include "Character.h"

class Item;
class CombatEngine;

// ============================================================================
// MONSTER
// ============================================================================
// Base class for all enemy types.
// Includes loot drop system - override in subclasses for specific drops.
// ============================================================================

class Monster : public Character {
public:
    Monster(std::string name, int hp, StatBlock stats);

    void LevelUp() override;
    
    // ========================================================================
    // LOOT SYSTEM
    // ========================================================================
    // Override these in subclasses for monster-specific drops
    
    // How much gold does this monster drop? (base + variance)
    virtual int GetGoldDrop(CombatEngine& engine) const;
    
    // What item does this monster drop? May return nullptr (no drop)
    virtual std::unique_ptr<Item> GetLootDrop(CombatEngine& engine) const;
    
protected:
    // Base gold range for this monster type
    int m_MinGold = 5;
    int m_MaxGold = 15;
    
    // Chance to drop loot (1-100)
    int m_LootDropChance = 20;  // 20% by default
};
