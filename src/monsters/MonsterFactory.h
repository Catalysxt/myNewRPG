#pragma once
#include <vector>
#include <memory>
#include <functional>
#include <string>

// Forward declarations
class Monster;
class CombatEngine;

// ============================================================================
// MONSTER FACTORY
// ============================================================================
// Spawns random monsters with:
// - Weighted spawn chances (e.g., Slime 60%, Orc 30%, Goblin 10%)
// - HP variance (±15% by default)
//
// USAGE:
//   MonsterFactory factory(combatEngine);
//   auto enemy = factory.SpawnRandom();
// ============================================================================

struct MonsterEntry {
    std::string name;
    int weight;                                          // Relative spawn weight
    float hpVariance;                                    // ±% HP variance (0.15 = ±15%)
    std::function <std::unique_ptr<Monster>()> create;    // Factory function
};

class MonsterFactory {
public:
    // Engine is used for random number generation
    explicit MonsterFactory(CombatEngine& engine);
    
    // Register a monster type with spawn weight and HP variance
    // The factory function creates a new instance of the monster
    void Register(
        const std::string& name,
        int weight,
        float hpVariance,
        std::function<std::unique_ptr<Monster>()> factoryFn
    );
    
    // Spawn a random monster based on weights
    // Returns nullptr if no monsters are registered
    std::unique_ptr<Monster> SpawnRandom();
    
    // Get total weight (for debugging/display)
    int GetTotalWeight() const { return m_TotalWeight; }
    
private:
    CombatEngine& m_Engine;
    std::vector<MonsterEntry> m_Entries;
    int m_TotalWeight = 0;
    
    // Apply HP variance to spawned monster
    void ApplyHPVariance(Monster& monster, float variance);
};
