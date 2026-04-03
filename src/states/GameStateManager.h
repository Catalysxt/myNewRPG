#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

#include <memory>
#include "IGameState.h"
#include "GameContext.h"

/**
 * @brief Manages game states and handles transitions between them.
 * 
 * This class:
 *   1. Owns the current active state (via unique_ptr)
 *   2. Holds the GameContext (shared resources)
 *   3. Runs the main game loop
 *   4. Handles state transitions cleanly
 * 
 * Usage:
 *   GameStateManager manager;
 *   manager.GetContext().player = myPlayer.get();  // Set up context
 *   manager.ChangeState(StateType::MainMenu);      // Set initial state
 *   manager.Run();                                  // Start game loop
 */
class GameStateManager {
public:
    GameStateManager();
    ~GameStateManager();

    // Prevent copying (we own unique resources)
    GameStateManager(const GameStateManager&) = delete;
    GameStateManager& operator=(const GameStateManager&) = delete;

    /**
     * @brief Get mutable reference to the game context.
     * Use this to set up shared resources before starting the game.
     */
    GameContext& GetContext() { return m_Context; }

    /**
     * @brief Change to a new state by type.
     * Calls OnExit() on current state, creates new state, calls OnEnter().
     */
    void ChangeState(StateType newStateType);

    /**
     * @brief Run the main game loop.
     * Continues until StateType::Quit is requested.
     */
    void Run();

    /**
     * @brief Check if the game is still running.
     */
    bool IsRunning() const { return m_IsRunning; }

private:
    /**
     * @brief Factory method to create a state from its type.
     * This is where concrete state classes are instantiated.
     */
    std::unique_ptr<IGameState> CreateState(StateType type);

    /**
     * @brief Process any pending state transition requests.
     */
    void ProcessTransitions();

private:
    GameContext m_Context;
    std::unique_ptr<IGameState> m_CurrentState;
    bool m_IsRunning = false;
};

#endif // GAMESTATEMANAGER_H
