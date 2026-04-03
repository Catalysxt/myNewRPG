#include "GameOverState.h"
#include "GameContext.h"
#include "Character.h"
#include <iostream>

void GameOverState::OnEnter(GameContext& context) {
    m_NextState = StateType::None;
    m_PlayerWon = context.playerWon;
    
    std::cout << "\n========================================" << std::endl;
    
    if (m_PlayerWon) {
        std::cout << "        *** VICTORY! ***               " << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "\nCongratulations, " << context.player->GetName() << "!" << std::endl;
        std::cout << "You emerged triumphant from battle!" << std::endl;
        std::cout << "\nCurrent Status:" << std::endl;
        std::cout << "  Level: " << context.player->GetLevel() << std::endl;
        std::cout << "  HP:    " << context.player->GetCurrentHP() << "/" << context.player->GetMaxHP() << std::endl;
        std::cout << "  Gold:  " << context.player->GetGold() << std::endl;
    } else {
        std::cout << "        *** DEFEAT ***                 " << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "\n" << context.player->GetName() << " has fallen in battle..." << std::endl;
        std::cout << "Your journey ends here... or does it?" << std::endl;
    }
    
    std::cout << "========================================" << std::endl;
}

void GameOverState::OnExit(GameContext& context) {
    // Nothing to clean up
}

void GameOverState::HandleInput(GameContext& context) {
    if (m_NextState != StateType::None) {
        return;
    }

    std::cout << "\nWhat would you like to do?" << std::endl;
    
    if (m_PlayerWon) {
        std::cout << "1. Continue Adventuring" << std::endl;
        std::cout << "2. Start New Game" << std::endl;
        std::cout << "3. Quit" << std::endl;
    } else {
        std::cout << "1. Try Again (New Game)" << std::endl;
        std::cout << "2. Quit" << std::endl;
    }
    
    std::cout << "Choose: ";
    
    int choice;
    std::cin >> choice;
    
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid input!" << std::endl;
        return;
    }
    
    if (m_PlayerWon) {
        switch (choice) {
            case 1:
                std::cout << "\nYou return to the training grounds..." << std::endl;
                m_NextState = StateType::Training;
                break;
            case 2:
                std::cout << "\nStarting a new adventure..." << std::endl;
                m_NextState = StateType::MainMenu;
                break;
            case 3:
                m_NextState = StateType::Quit;
                break;
            default:
                std::cout << "Invalid choice!" << std::endl;
                break;
        }
    } else {
        switch (choice) {
            case 1:
                std::cout << "\nStarting a new adventure..." << std::endl;
                m_NextState = StateType::MainMenu;
                break;
            case 2:
                m_NextState = StateType::Quit;
                break;
            default:
                std::cout << "Invalid choice!" << std::endl;
                break;
        }
    }
}

void GameOverState::Update(GameContext& context) {
    if (m_NextState != StateType::None) {
        context.RequestStateChange(m_NextState);
    }
}

void GameOverState::Render(GameContext& context) {
    // Console rendering handled in HandleInput
}
