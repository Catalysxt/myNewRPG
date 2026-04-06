#include <iostream>
#include <memory>

// Core game systems
#include "Character.h"
#include "Merchant.h"
#include "CombatEngine.h"
#include "MonsterFactory.h"
#include "EventBus.h"
#include "PricingStrategy.h"

// Monsters (for factory registration)
#include "Slime.h"
#include "Orc.h"
#include "Goblin.h"

// Game State Machine
#include "GameStateManager.h"
#include "GameContext.h"

/**
 * @brief Main entry point for the RPG game.
 * 
 * This file now serves as the bootstrap/setup:
 *   1. Subscribe to events (Observer pattern)
 *   2. Create and own shared resources
 *   3. Initialize the Game State Machine
 *   4. Run the game loop
 * 
 * All game logic has been moved to state classes.
 */
int main() {
    // =========================================================================
    // EVENT SUBSCRIPTIONS (Observer Pattern)
    // =========================================================================
    // These lambdas are called whenever their event type is published.
    // This decouples logging from game logic.
    
    EventBus::Instance().Subscribe(EventType::CharacterDamaged, [](const GameEvent& e) {
        std::cout << "[LOG] " << e.target->GetName() << " took " << e.value << " damage" << std::endl;
    });
    
    EventBus::Instance().Subscribe(EventType::CharacterDied, [](const GameEvent& e) {
        std::cout << "[LOG] " << e.target->GetName() << " has fallen!" << std::endl;
    });
    
    EventBus::Instance().Subscribe(EventType::CharacterHealed, [](const GameEvent& e) {
        std::cout << "[LOG] " << e.target->GetName() << " healed for " << e.value << " HP" << std::endl;
    });
    
    EventBus::Instance().Subscribe(EventType::CharacterLeveledUp, [](const GameEvent& e) {
        std::cout << "[LOG] " << e.target->GetName() << " reached level " << e.value << "!" << std::endl;
    });

    // =========================================================================
    // SHARED RESOURCES (Owned by main, accessed via GameContext)
    // =========================================================================
    // main() owns these objects via unique_ptr or stack allocation.
    // GameContext holds non-owning raw pointers for state access.
    
    // Merchant with pricing strategy
    Merchant greedyMerchant("Grimbold the Greedy", 
                            std::make_unique<GreedyMerchantPricing>());
    
    // Combat calculation engine
    CombatEngine combatEngine;
    
    // Monster factory with registered types
    MonsterFactory monsterFactory(combatEngine);
    monsterFactory.Register("Slime", 60, 0.15f, []() { return std::make_unique<Slime>(); });
    monsterFactory.Register("Orc", 30, 0.15f, []() { return std::make_unique<Orc>(); });
    monsterFactory.Register("Goblin", 10, 0.15f, []() { return std::make_unique<Goblin>(); });

    // =========================================================================
    // GAME STATE MACHINE SETUP
    // =========================================================================
    GameStateManager gameManager;
    
    // Inject shared resources into context
    // Note: player is nullptr initially - MainMenuState will create it
    GameContext& context = gameManager.GetContext();
    
    context.merchant = &greedyMerchant;
    context.combatEngine = &combatEngine;
    context.monsterFactory = &monsterFactory;
    
    // Set initial state and run the game!
    gameManager.ChangeState(StateType::MainMenu);
    gameManager.Run();

    return 0;
}
