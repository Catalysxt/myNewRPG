#include <iostream>
#include "MonsterFactory.h"
#include "Monster.h"
#include "CombatEngine.h"

MonsterFactory::MonsterFactory(CombatEngine& engine) 
    : m_Engine(engine) {
}

void MonsterFactory::Register(
    const std::string& name,
    int weight,
    float hpVariance,
    std::function<std::unique_ptr<Monster>()> factoryFn
) {
    m_Entries.push_back({name, weight, hpVariance, std::move(factoryFn)});
    m_TotalWeight += weight;
}

std::unique_ptr<Monster> MonsterFactory::SpawnRandom() {
    if (m_Entries.empty() || m_TotalWeight <= 0) {
        return nullptr;
    }
    
    // Roll a random number from 1 to total weight
    std::uniform_int_distribution<int> dist(1, m_TotalWeight);
    int roll = dist(m_Engine.GetRandomEngine());
    
    // Find which monster the roll corresponds to
    int cumulative = 0;
    for (const auto& entry : m_Entries) {
        cumulative += entry.weight;
        if (roll <= cumulative) {
            // Create the monster
            auto monster = entry.create();
            
            // Apply HP variance
            ApplyHPVariance(*monster, entry.hpVariance);
            
            std::cout << "A wild " << monster->GetClassName() << " appears!" << std::endl;
            return monster;
        }
    }
    
    // Should never reach here, but fallback to first entry
    return m_Entries[0].create();
}

void MonsterFactory::ApplyHPVariance(Monster& monster, float variance) {
    if (variance <= 0.0f) return;
    
    // Get current HP
    int baseHP = monster.GetMaxHP();
    
    // Calculate variance range: e.g., for 15% variance on 100 HP: 85-115
    float minMult = 1.0f - variance;
    float maxMult = 1.0f + variance;
    
    // Roll for multiplier
    std::uniform_real_distribution<float> dist(minMult, maxMult);
    float multiplier = dist(m_Engine.GetRandomEngine());
    
    // Apply variance
    int newHP = static_cast<int>(baseHP * multiplier);
    if (newHP < 1) newHP = 1;  // Minimum 1 HP
    
    // Update monster HP (need to access through protected members or add a method)
    monster.SetMaxHP(newHP);
    monster.Heal(newHP);  // Heal to new max
}
