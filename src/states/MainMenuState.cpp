#include "MainMenuState.h"
#include "GameContext.h"
#include "Character.h"
#include "Warrior.h"
#include "Wizard.h"
#include "Cleric.h"
#include "Rogue.h"
#include "Potion.h"
#include "EventBus.h"
#include <iostream>
#include <memory>
#include <limits>

// Local enum for class selection (matches original main.cpp)
enum class CharacterType {
    Warrior = 1,
    Wizard,
    Cleric,
    Rogue
};

// Factory function (moved from main.cpp)
static std::unique_ptr<Character> CreateCharacter(CharacterType type, const std::string& name) {
    switch (type) {
        case CharacterType::Warrior: return std::make_unique<Warrior>(name);
        case CharacterType::Wizard:  return std::make_unique<Wizard>(name);
        case CharacterType::Cleric:  return std::make_unique<Cleric>(name);
        case CharacterType::Rogue:   return std::make_unique<Rogue>(name);
        default: return nullptr;
    }
}

void MainMenuState::OnEnter(GameContext& context) {
    m_CharacterCreated = false;
    
    std::cout << "========================================" << std::endl;
    std::cout << "       WELCOME TO THE CPP RPG           " << std::endl;
    std::cout << "========================================" << std::endl;
}

void MainMenuState::OnExit(GameContext& context) {
    // Nothing to clean up
}

void MainMenuState::HandleInput(GameContext& context) {
    if (m_CharacterCreated) {
        return; // Already done, waiting for transition
    }

    // Display class options
    std::cout << "Choose your class:" << std::endl;
    std::cout << "1. Warrior (High HP, Strong Physical)" << std::endl;
    std::cout << "2. Wizard  (Low HP, Strong Magic)" << std::endl;
    std::cout << "3. Cleric  (Balanced, High Resistance)" << std::endl;
    std::cout << "4. Rogue   (Fast, High Agility)" << std::endl;
    std::cout << "========================================" << std::endl;
    
    int choice = 0;
    std::cout << "Enter selection (1-4): ";
    std::cin >> choice;

    // Validate input
    if (std::cin.fail() || choice < 1 || choice > 4) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid choice! Defaulting to Warrior." << std::endl;
        choice = 1;
    }

    // Get name
    std::string name;
    std::cout << "Enter your hero's name: ";
    std::cin >> name;

    // Create character
    CharacterType selectedType = static_cast<CharacterType>(choice);
    
    // IMPORTANT: We need to store the character somewhere persistent.
    // The GameContext holds a non-owning pointer, so we need the manager
    // to own the character. For now, we use a static unique_ptr.
    // This will be refactored when we update main.cpp.
    std::unique_ptr<Character> player;
    player = CreateCharacter(selectedType, name);
    
    if (player) {
        context.gameStateManager->MoveCharacter(std::move(player));
        
        std::cout << "\n----------------------------------------" << std::endl;
        std::cout << "You have chosen the path of the " << context.player->GetClassName() << "!" << std::endl;
        std::cout << "Character Created Successfully!" << std::endl;
        std::cout << "Name:  " << context.player->GetName() << std::endl;
        std::cout << "HP:    " << context.player->GetCurrentHP() << "/" << context.player->GetMaxHP() << std::endl;
        std::cout << "Stats: [Str: " << context.player->GetStats().GetStrength()
                  << ", Int: " << context.player->GetStats().GetIntellect()
                  << ", Agi: " << context.player->GetStats().GetAgility() << "]" << std::endl;
        
        // Give starting item
        std::cout << "\n[Gift] You receive a Health Potion for your journey!" << std::endl;
        context.player->AddItem(std::make_unique<Potion>(30));
        
        m_CharacterCreated = true;
    }
}

void MainMenuState::Update(GameContext& context) {
    // If character was created, request transition to shop
    if (m_CharacterCreated) {
        context.RequestStateChange(StateType::Shop);
    }
}

void MainMenuState::Render(GameContext& context) {
    // For console-based game, rendering is done during HandleInput
    // In a graphical game, this would draw the menu UI
}
