#include "TrainingAreaState.h"
#include "GameContext.h"
#include "Character.h"
#include <iostream>

void TrainingAreaState::OnEnter(GameContext& context) {
    m_NextState = StateType::None;
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "       TRAINING AREA                    " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "You arrive at the training grounds." << std::endl;
    std::cout << "A wooden training dummy stands before you." << std::endl;
}

void TrainingAreaState::OnExit(GameContext& context) {
    // Clean up
}

void TrainingAreaState::HandleInput(GameContext& context) {
    if (m_NextState != StateType::None) {
        return; // Already made a choice
    }

    std::cout << "\n----------------------------------------" << std::endl;
    std::cout << context.player->GetName() << " the " << context.player->GetClassName() << std::endl;
    std::cout << "HP: " << context.player->GetCurrentHP() << "/" << context.player->GetMaxHP() 
              << "  |  Gold: " << context.player->GetGold() << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "What would you like to do?" << std::endl;
    std::cout << "1. Fight Training Dummy (Practice)" << std::endl;
    std::cout << "2. Enter the Dungeon (Real Combat!)" << std::endl;
    std::cout << "3. Visit Shop" << std::endl;
    std::cout << "4. View Stats & Inventory" << std::endl;
    std::cout << "5. Quit Game" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Choose: ";
    
    int choice;
    std::cin >> choice;
    
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid input!" << std::endl;
        return;
    }
    
    switch (choice) {
        case 1:
            std::cout << "\nYou prepare to spar with the training dummy..." << std::endl;
            context.isTrainingCombat = true;
            m_NextState = StateType::Combat;
            break;
            
        case 2:
            std::cout << "\nYou steel yourself and enter the dungeon..." << std::endl;
            context.isTrainingCombat = false;
            m_NextState = StateType::Combat;
            break;
            
        case 3:
            m_NextState = StateType::Shop;
            break;
            
        case 4:
            // Display detailed stats
            std::cout << "\n========================================" << std::endl;
            std::cout << "           CHARACTER STATUS             " << std::endl;
            std::cout << "========================================" << std::endl;
            std::cout << "Name:  " << context.player->GetName() << std::endl;
            std::cout << "Class: " << context.player->GetClassName() << std::endl;
            std::cout << "Level: " << context.player->GetLevel() << std::endl;
            std::cout << "HP:    " << context.player->GetCurrentHP() << "/" << context.player->GetMaxHP() << std::endl;
            std::cout << "RP:    " << context.player->GetCurrentRP() << "/" << context.player->GetMaxRP() << std::endl;
            std::cout << "Gold:  " << context.player->GetGold() << std::endl;
            std::cout << "\n--- STATS ---" << std::endl;
            std::cout << "Strength:  " << context.player->GetStats().GetStrength() << std::endl;
            std::cout << "Intellect: " << context.player->GetStats().GetIntellect() << std::endl;
            std::cout << "Agility:   " << context.player->GetStats().GetAgility() << std::endl;
            std::cout << "Armor:     " << context.player->GetStats().GetArmor() << std::endl;
            std::cout << "\n--- INVENTORY ---" << std::endl;
            if (context.player->HasItems()) {
                context.player->PrintInventory();
            } else {
                std::cout << "(Empty)" << std::endl;
            }
            std::cout << "========================================" << std::endl;
            // Don't transition, stay in training area
            break;
            
        case 5:
            std::cout << "\nAre you sure you want to quit? (1=Yes, 0=No): ";
            int confirm;
            std::cin >> confirm;
            if (confirm == 1) {
                m_NextState = StateType::Quit;
            }
            break;
            
        default:
            std::cout << "Invalid choice!" << std::endl;
            break;
    }
}

void TrainingAreaState::Update(GameContext& context) {
    if (m_NextState != StateType::None) {
        context.RequestStateChange(m_NextState);
    }
}

void TrainingAreaState::Render(GameContext& context) {
    // Console rendering handled in HandleInput
}
