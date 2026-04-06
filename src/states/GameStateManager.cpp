#include "GameStateManager.h"
#include "MainMenuState.h"
#include "ShopState.h"
#include "TrainingAreaState.h"
#include "CombatState.h"
#include "GameOverState.h"
#include <iostream>

GameStateManager::GameStateManager() : m_Context(), m_CurrentState(nullptr), m_IsRunning(false) {
    m_Context.gameStateManager = this;
}

GameStateManager::~GameStateManager() {
    // If there's a current state, call OnExit for clean shutdown
    if (m_CurrentState) {
        m_CurrentState->OnExit(m_Context);
    }
    // unique_ptr automatically deletes m_CurrentState
}

void GameStateManager::ChangeState(StateType newStateType) {
    // Handle quit request
    if (newStateType == StateType::Quit) {
        m_IsRunning = false;
        if (m_CurrentState) {
            m_CurrentState->OnExit(m_Context);
            m_CurrentState.reset();
        }
        return;
    }

    // Create the new state BEFORE exiting old one
    // This ensures we have a valid state to transition to
    std::unique_ptr<IGameState> newState = CreateState(newStateType);
    
    if (!newState) {
        std::cerr << "Error: Failed to create state of type " 
                  << static_cast<int>(newStateType) << std::endl;
        return;
    }

    // Exit current state (if any)
    if (m_CurrentState) {
        m_CurrentState->OnExit(m_Context);
    }

    // Swap to new state (old state is automatically deleted by unique_ptr)
    m_CurrentState = std::move(newState);

    // Enter new state
    m_CurrentState->OnEnter(m_Context);
}

void GameStateManager::Run() {
    m_IsRunning = true;

    // Main game loop
    while (m_IsRunning && m_CurrentState) {
        // 1. Handle input
        m_CurrentState->HandleInput(m_Context);

        // 2. Update logic
        m_CurrentState->Update(m_Context);

        // 3. Render (for console, this is printing output)
        m_CurrentState->Render(m_Context);

        // 4. Check for state transition requests
        ProcessTransitions();
    }

    std::cout << "\nThanks for playing!" << std::endl;
}

void GameStateManager::ProcessTransitions() {
    if (m_Context.HasPendingTransition()) {
        StateType nextState = m_Context.requestedState;
        m_Context.ClearTransitionRequest();
        ChangeState(nextState);
    }
}

std::unique_ptr<IGameState> GameStateManager::CreateState(StateType type) {
    // Factory pattern: create concrete state based on enum 
    switch (type) {
        case StateType::MainMenu:
            return std::make_unique<MainMenuState>();
        case StateType::Shop:
            return std::make_unique<ShopState>();
        case StateType::Training:
            return std::make_unique<TrainingAreaState>();
        case StateType::Combat:
            return std::make_unique<CombatState>();
        case StateType::GameOver:
            return std::make_unique<GameOverState>();
        default:
            return nullptr;
    }
}

void GameStateManager::MoveCharacter(std::unique_ptr<Character> player) {
    m_player = std::move(player);
    m_Context.player = m_player.get(); // Give it a raw pointer from the unique pointer 
}
