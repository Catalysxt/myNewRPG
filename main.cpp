#include <iostream>
#include <memory>
#include <string>
#include <limits> // Required for clearing input buffer if needed

#include "StatBlock.h"
#include "Character.h"
#include "Warrior.h"
#include "Wizard.h"
#include "Cleric.h"
#include "Rogue.h"
#include "Orc.h"
#include "Slime.h"
#include "Potion.h" 
#include "Merchant.h" // NEW: For Shop System

enum class CharacterType {
    Warrior = 1, // Start at 1 to match user input convenience
    Wizard,
    Cleric,
    Rogue
};

// Factory Function
std::unique_ptr<Character> CreateCharacter(CharacterType type, std::string name) {
    switch (type) {
        case CharacterType::Warrior: return std::make_unique<Warrior>(name);
        case CharacterType::Wizard:  return std::make_unique<Wizard>(name);
        case CharacterType::Cleric:  return std::make_unique<Cleric>(name);
        case CharacterType::Rogue:   return std::make_unique<Rogue>(name);
        default: return nullptr;
    }
}

// --- Shop Interaction Function ---
void VisitShop(Character& player, Merchant& merchant) {

    bool shopping = true;
    while (shopping) {
        std::cout << "\nYour Gold: " << player.GetGold() << std::endl;
        merchant.DisplayStock();
        std::cout << "Enter item # to buy (0 to exit): ";
        
        int choice;
        std::cin >> choice;
        
        if (choice == 0) {
            shopping = false;
        } else {
            TradeResult result = merchant.SellItem(player, choice - 1); // 0-indexed
            switch (result) {
                case TradeResult::Success:
                    // Message already printed by Merchant
                    break;
                case TradeResult::InsufficientGold:
                    // Message already printed by SpendGold
                    break;
                case TradeResult::ItemNotFound:
                    std::cout << "That item is not available." << std::endl;
                    break;
                case TradeResult::InventoryFull:
                    std::cout << "Your inventory is full!" << std::endl;
                    break;
            }
        }
    }
    std::cout << "You leave the shop." << std::endl;
}

enum class BattleState { 
    PlayerTurn, 
    EnemyTurn, 
    Victory, 
    Defeat 
};

// Main Gameplay loop

void PlayBattle(Character* player, Monster* enemy, Merchant& merchant) {
    BattleState state = BattleState::PlayerTurn;
    
    std::cout << "\n--- BATTLE START ---" << std::endl;
    std::cout << player->GetName() << " VS " << enemy->GetClassName() << "!" << std::endl;

    while (state != BattleState::Victory && state != BattleState::Defeat) {
        if (state == BattleState::PlayerTurn) {
            std::cout << "\n=======================================================" << std::endl;
            std::cout << "[Your Turn] HP: " << player->GetCurrentHP() << "/" << player->GetMaxHP() << std::endl;
            std::cout << "1. Attack" << std::endl;
            std::cout << "2. Heal (Not implemented)" << std::endl;
            std::cout << "3. Inventory" << std::endl; 
            std::cout << "4. Shop" << std::endl;
            std::cout << "=======================================================" << std::endl;
            std::cout << "Choose: ";
            
            int action;
            if (!(std::cin >> action)) {
                std::cin.clear();
                std::cin.ignore(10000, '\n'); 
                action = 0;
            }

            if (action == 1) {
                // ATTACK LOGIC
                int dmg = player->GetStats().GetStrength();
                std::cout << "You attack the " << enemy->GetClassName() << " for " << dmg << " damage!" << std::endl;
                enemy->TakeDamage(dmg);
                std::cout << "Enemy HP: " << enemy->GetCurrentHP() << "/" << enemy->GetMaxHP() << std::endl; 

                if (enemy->GetCurrentHP() <= 0) {
                    state = BattleState::Victory;
                } else {
                    state = BattleState::EnemyTurn;
                }
            } 
            else if (action == 3) { // INVENTORY LOGIC
                 if (player->HasItems()) {
                     player->PrintInventory();
                     std::cout << "0. Back" << std::endl;
                     std::cout << "Enter item # to use: ";
                     
                     int itemIndex;
                     std::cin >> itemIndex;
                     
                     if (itemIndex > 0 && itemIndex <= 100) { // Limit sanity check
                         player->UseItem(itemIndex - 1);
                         state = BattleState::EnemyTurn; // Using item takes turn
                     } else {
                         std::cout << "Cancelled." << std::endl;
                     }
                 } else {
                     std::cout << "\nYou have no items!" << std::endl;
                 }
            }

            else if (action == 4) { // Shop logic
                VisitShop(*player, merchant);   
            }

            else {
                std::cout << "Invalid action! You stumble." << std::endl;
                state = BattleState::EnemyTurn;
            }
        } 
        else if (state == BattleState::EnemyTurn) {
            std::cout << "\n=======================================================" << std::endl;
            std::cout << "[Enemy Turn]" << std::endl;
            std::cout << "=======================================================" << std::endl;
            
            // Simple AI: Enemy attack value is equivalent to its Strength attribute
            int dmg = enemy->GetStats().GetStrength();
            std::cout << "The " << enemy->GetClassName() << " attacks you for " << dmg << " damage!" << std::endl;
            player->TakeDamage(dmg);

            if (player->GetCurrentHP() <= 0) {
                state = BattleState::Defeat;
            } else {
                state = BattleState::PlayerTurn;
            }
        }
    }

    if (state == BattleState::Victory) {
        std::cout << "\nVICTORY! You defeated the " << enemy->GetClassName() << "!" << std::endl;
        player->GainXP(50); // Reward
    } else {
        std::cout << "\nDEFEAT! You have fallen..." << std::endl;
    }
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "       WELCOME TO THE CPP RPG           " << std::endl;
    std::cout << "========================================" << std::endl;

    // 1. Character Selection
    std::cout << "Choose your class:" << std::endl;
    std::cout << "1. Warrior (High HP, Strong Physical)" << std::endl;
    std::cout << "2. Wizard  (Low HP, Strong Magic)" << std::endl;
    std::cout << "3. Cleric  (Balanced, High Resistance)" << std::endl;
    std::cout << "4. Rogue   (Fast, High Agility)" << std::endl;
    std::cout << "========================================" << std::endl;
    
    int choice = 0;
    std::cout << "Enter selection (1-4): ";
    std::cin >> choice;

    // Basic validation
    if (choice < 1 || choice > 4) {
        std::cout << "Invalid choice! Defaulting to Warrior." << std::endl;
        choice = 1;
    }

    std::string name;
    std::cout << "Enter your hero's name: ";
    std::cin >> name;

    // Convert int to Enum. We'll use this to create the derived class (the character)
    CharacterType selectedType = static_cast<CharacterType>(choice);

    // Factory handles creation of Character objects
    std::unique_ptr<Character> player = CreateCharacter(selectedType, name);

    if (player) {
        std::cout << "\n----------------------------------------" << std::endl;
        std::cout << "You have chosen the path of the " << player->GetClassName() << "!" << std::endl;
        std::cout << "Character Created Successfully!" << std::endl;
        std::cout << "Name:  " << player->GetName() << std::endl; 
        
        std::cout << "HP:    " << player->GetCurrentHP() << "/" << player->GetMaxHP() << std::endl;
        std::cout << "Stats: [Str: " << player->GetStats().GetStrength() 
                  << ", Int: " << player->GetStats().GetIntellect()
                  << ", Agi: " << player->GetStats().GetAgility() << "]" << std::endl;

        // Give the player a starting item!
        std::cout << "\n[Gift] You receive a Health Potion for your journey!" << std::endl;
        player->AddItem(std::make_unique<Potion>(30));

        // --- SHOP PHASE ---
        std::cout << "\n----------------------------------------" << std::endl;
        std::cout << "You encounter a traveling merchant!" << std::endl;
        
        // Justification: Strategy Pattern + Dependency Injection
        // We create a GreedyMerchantPricing strategy and inject it into the Merchant.
        Merchant greedyMerchant("Grimbold the Greedy", 
                                std::make_unique<GreedyMerchantPricing>());
        
        VisitShop(*player, greedyMerchant); 

        // --- COMBAT PHASE ---
        std::cout << "\n----------------------------------------" << std::endl;
        std::cout << "A wild monster appears!" << std::endl;

        // Create Monster
        std::unique_ptr<Monster> enemy = std::make_unique<Orc>();
        std::cout << "It's an " << enemy->GetClassName() << " with " << enemy->GetMaxHP() << " HP." << std::endl;
        
        // Start the interactive battle
        PlayBattle(player.get(), enemy.get(), greedyMerchant);
    }

    return 0;
}
