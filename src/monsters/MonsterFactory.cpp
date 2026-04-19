#include <iostream>
#include <numeric>
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
    m_DistributionDirty = true;  // Mark for rebuild on next spawn
}

// ============================================================================
// REBUILD DISTRIBUTION
// ============================================================================
// Extracts the weights from all entries and passes them to
// std::discrete_distribution, which pre-computes an internal data
// structure for O(1) index selection.
// Called lazily — only when SpawnRandom() detects the dirty flag.
// ============================================================================

void MonsterFactory::RebuildDistribution() {
    std::vector<double> weights;
    weights.reserve(m_Entries.size());

    for (const auto& entry : m_Entries) {
        weights.push_back(static_cast<double>(entry.weight));
    }

    m_Distribution = std::discrete_distribution<int>(weights.begin(), weights.end());
    m_DistributionDirty = false;
}

// ============================================================================
// SPAWN RANDOM
// ============================================================================
// Uses std::discrete_distribution to select a monster index directly.
// The distribution handles all weight math internally.
//
// OLD approach (O(n) per spawn):
//   Roll a number, then walk through every entry accumulating weights
//   until the roll is covered. For 3 monsters this is trivial, but
//   for hundreds it becomes a linear scan every single spawn.
//
// NEW approach (O(1) per spawn after O(n) setup):
//   std::discrete_distribution pre-computes lookup tables during
//   construction. Each call to operator() returns a weighted random
//   index in constant time.
// ============================================================================

std::unique_ptr<Monster> MonsterFactory::SpawnRandom() {
    if (m_Entries.empty()) {
        return nullptr;
    }
    
    // Rebuild the distribution only if entries have changed
    if (m_DistributionDirty) {
        RebuildDistribution();
    }
    
    // Select a random index — weighted, O(1)
    int index = m_Distribution(m_Engine.GetRandomEngine());
    
    const auto& entry = m_Entries[index];
    
    // Create the monster
    auto monster = entry.create();
    
    // Apply HP variance
    ApplyHPVariance(*monster, entry.hpVariance);
    
    std::cout << "A wild " << monster->GetClassName() << " appears!" << std::endl;
    return monster;
}

int MonsterFactory::GetTotalWeight() const {
    return std::accumulate(m_Entries.begin(), m_Entries.end(), 0,
        [](int sum, const MonsterEntry& e) { return sum + e.weight; });
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
