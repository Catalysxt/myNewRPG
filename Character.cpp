#include <iostream>
#include <cmath> // Required for std::pow
#include "Character.h"
#include "Item.h"
#include "CombatEngine.h"

Character::Character(std::string name, int hp, StatBlock stats)
    : m_Name(name), m_MaxHP(hp), m_CurrentHP(hp), m_Stats(stats), 
      m_Level(1), m_CurrentXP(0), m_MaxXP(100), m_Gold(100),
      m_CurrentRP(stats.GetMaxResourcePoints()),  // Start with full RP. We construct Statblock with 100 RP
      m_Status(StatusEffect::None) {
}

Character::~Character() {
}

void Character::TakeDamage(int damage) {
    m_CurrentHP -= damage;
    if (m_CurrentHP < 0) {
        m_CurrentHP = 0;
    }
    std::cout << m_Name << " takes " << damage << " damage." << std::endl;
}

void Character::Heal(int amount) {
    m_CurrentHP += amount;
    if (m_CurrentHP > m_MaxHP) {
        m_CurrentHP = m_MaxHP; // Prevent healing past MaxHP
    }
    std::cout << m_Name << " heals for " << amount << " HP!" << std::endl;
}

// Getters

std::string Character::GetName() {
    return m_Name;
}

int Character::GetCurrentHP() const {
    return m_CurrentHP;
}

int Character::GetMaxHP() const {
    return m_MaxHP;
}

int Character::GetLevel() const {
    return m_Level;
}

StatBlock Character::GetStats() const {
    return m_Stats;
}

// Base damage for unarmed attacks. Override this in subclasses or when implementing a weapon system. 
// Used by CombatEngine::CalculateDamage()
int Character::GetBaseDamage() const {
    return 5; // Default
}

const DamageStrategy& Character::GetDamageStrategy() const {
    return *m_DamageStrategy;
}

// =========================================================================
// RESOURCE POINT SYSTEM
// =========================================================================

int Character::GetCurrentRP() const {
    return m_CurrentRP;
}

int Character::GetMaxRP() const {
    return m_Stats.GetMaxResourcePoints();
}

void Character::SpendRP(int amount) {
    m_CurrentRP -= amount;
    if (m_CurrentRP < 0) m_CurrentRP = 0;
}

void Character::RestoreRP(int amount) {
    m_CurrentRP += amount;
    int max = GetMaxRP();
    if (m_CurrentRP > max) {m_CurrentRP = max;}
    std::cout << m_Name << " restored " << amount << " RP!" << std::endl;
}

// =========================================================================
// STATUS EFFECT SYSTEM
// =========================================================================

void Character::ApplyStatus(StatusEffect effect) {
    m_Status |= effect;
}

void Character::RemoveStatus(StatusEffect effect) {
    m_Status &= ~effect;
}

bool Character::HasStatus(StatusEffect effect) const {
    return HasEffect(m_Status, effect);
}

void Character::ClearAllStatus() {
    m_Status = StatusEffect::None;
}

StatusEffect Character::GetCurrentStatus() const {
    return m_Status;
}

// =========================================================================
// SPECIAL ABILITY SYSTEM (Template Method Pattern)
// =========================================================================
// This is the TEMPLATE METHOD - it controls the algorithm skeleton:
//   1. Check if enough RP
//   2. Deduct RP
//   3. Call the subclass hook (ExecuteSpecialAbility)

void Character::PerformSpecialAbility(Character* target, CombatEngine& engine) {
    int cost = GetAbilityCost();
    
    if (m_CurrentRP < cost) {
        std::cout << m_Name << " doesn't have enough RP! (Have: " 
                  << m_CurrentRP << ", Need: " << cost << ")" << std::endl;
        return;
    }
    
    SpendRP(cost);
    ExecuteSpecialAbility(target, engine);
}

// Default implementation - does nothing (subclasses override this)
void Character::ExecuteSpecialAbility(Character* /*target*/, CombatEngine& /*engine*/) {
    std::cout << m_Name << " has no special ability!" << std::endl;
}

void Character::GainXP(int amount) {
    m_CurrentXP += amount;
    std::cout << m_Name << " gained " << amount << " XP!" << std::endl;
    if (m_CurrentXP >= m_MaxXP) {
        LevelUp();
    }
}

void Character::LevelUp() {
    // Basic Level Up Logic
    m_CurrentXP -= m_MaxXP; // Overflow XP carries over
    m_Level++;
    
    // We explicitly calculate the next XP threshold using an exponential formula.
    m_MaxXP = static_cast<int>(100 * std::pow(1.5, m_Level));
    
    // What actually changes on a Level Up

    // 1. Increase HP
    m_MaxHP += 10; 
    m_CurrentHP = m_MaxHP; // Heal on level up

    // 2. Default Stats Increase (Can be overridden by subclasses for specific growth)
    m_Stats.IncreaseStats(1, 1, 1, 0, 0); 

    std::cout << "LEVEL UP! " << m_Name << " is now Level " << m_Level << "!" << std::endl;
    std::cout << "HP Increased to " << m_MaxHP << ". Stats increased!" << std::endl;
}

// --- Inventory System Implementation ---

void Character::AddItem(std::unique_ptr<Item> item) {
    std::cout << m_Name << " received: " << item->GetName() << std::endl;
    m_Inventory.push_back(std::move(item)); // std::move is used to transfer ownership of item to inventory since item is a unique ptr
}

bool Character::HasItems() {
    return !m_Inventory.empty();
}

void Character::PrintInventory() {
    std::cout << "\n--- Inventory ---" << std::endl;
    if (m_Inventory.empty()) {
        std::cout << "(Empty)" << std::endl;
        return;
    }
    
    for (size_t i = 0; i < m_Inventory.size(); ++i) {
        std::cout << i + 1 << ". " << m_Inventory[i]->GetName() 
                  << " (" << m_Inventory[i]->GetDescription() << ")" << std::endl;
    }
}

void Character::UseItem(int index) {
    if (index < 0 || index >= static_cast<int>(m_Inventory.size())) {
        std::cout << "Invalid item selection!" << std::endl;
        return;
    }

    // Access the item
    // We use .get() to pass the raw pointer because Use() expects a pointer, not unique_ptr
    m_Inventory[index]->Use(this);

    // If consumable, remove it
    if (m_Inventory[index]->IsConsumable()) {
        // std::vector::erase needs an iterator. 
        // m_Inventory.begin() + index gives us the iterator to the item.
        m_Inventory.erase(m_Inventory.begin() + index);
    }
}

// --- Gold System Implementation ---

int Character::GetGold() const {
    // Justification: Const Correctness
    // This getter does not modify the object, so it's marked const.
    return m_Gold;
}

void Character::AddGold(int amount) {
    // Justification: Encapsulation
    // Instead of exposing m_Gold directly, we use a method.
    // This allows us to add validation or effects later (e.g., "gold cap").
    m_Gold += amount;
    std::cout << m_Name << " received " << amount << " gold. Total: " << m_Gold << std::endl;
}

bool Character::SpendGold(int amount) {
    if (m_Gold < amount) {
        std::cout << m_Name << " doesn't have enough gold! (Have: " << m_Gold << ", Need: " << amount << ")" << std::endl;
        return false;
    }
    m_Gold -= amount; // If we purchase an item, decrement Gold
    return true;
}
