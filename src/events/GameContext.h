#ifndef GAMECONTEXT_H
#define GAMECONTEXT_H

// Forward declarations - we only need pointers, not full definitions.
// This avoids circular #include issues and speeds up compilation.
class Character;
class Merchant;
class CombatEngine;
class MonsterFactory;

/**
 * @brief Enumeration of all possible game states.
 * 
 * States use this enum to REQUEST a transition. The GameStateManager
 * then handles the actual transition. This keeps states decoupled -
 * CombatState doesn't need to #include "GameOverState.h".
 */
enum class StateType {
    None,           // No transition requested
    MainMenu,       // Character creation
    Shop,           // Merchant interaction
    Training,       // Training area hub
    Combat,         // Battle system
    GameOver,       // Victory/defeat screen
    Quit            // Exit the game
};

/**
 * @brief Container for shared resources accessible to all game states.
 * 
 * KEY CONCEPT: Non-owning raw pointers
 * =====================================
 * These are intentionally RAW POINTERS, not smart pointers.
 * 
 * Why? Because GameContext does NOT own these objects:
 *   - main() creates and owns the Character, Merchant, etc.
 *   - GameContext just provides ACCESS to them for states
 *   - When main() ends, it cleans up the objects it owns
 * 
 * This follows the C++ Core Guidelines:
 *   - unique_ptr = "I own this exclusively"
 *   - shared_ptr = "I share ownership with others"
 *   - raw pointer = "I just need access, someone else owns it"
 * 
 * Using shared_ptr here would be wrong because:
 *   1. It implies shared ownership when there is none
 *   2. It adds unnecessary overhead (reference counting)
 *   3. It can cause circular reference issues
 */
struct GameContext {
    // =========================================================================
    // Shared Resources (non-owning pointers)
    // =========================================================================
    Character* player = nullptr;
    Merchant* merchant = nullptr;
    CombatEngine* combatEngine = nullptr;
    MonsterFactory* monsterFactory = nullptr;

    // =========================================================================
    // State Transition Request
    // =========================================================================
    // States set this to request a transition. The GameStateManager checks
    // this after each loop iteration and performs the transition if needed.
    StateType requestedState = StateType::None;

    // =========================================================================
    // Inter-State Communication
    // =========================================================================
    // Data that needs to pass between states goes here.
    
    // Set by CombatState, read by GameOverState
    bool playerWon = false;
    
    // Set by TrainingState to indicate if this is practice combat
    bool isTrainingCombat = false;

    // =========================================================================
    // Helper Methods
    // =========================================================================
    
    /**
     * @brief Request a state transition.
     * The transition won't happen immediately - it's processed by the manager
     * after the current state's update cycle completes.
     */
    void RequestStateChange(StateType newState) {
        requestedState = newState;
    }

    /**
     * @brief Check if a state transition has been requested.
     */
    bool HasPendingTransition() const {
        return requestedState != StateType::None;
    }

    /**
     * @brief Clear the transition request (called by manager after processing).
     */
    void ClearTransitionRequest() {
        requestedState = StateType::None;
    }
};

#endif // GAMECONTEXT_H
