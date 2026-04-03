#ifndef IGAMESTATE_H
#define IGAMESTATE_H

// Forward declaration to avoid circular includes
struct GameContext;

/**
 * @brief Abstract base class for all game states.
 * 
 * This is the core of the State Design Pattern. Each concrete state
 * (MainMenu, Combat, Shop, etc.) inherits from this interface and
 * implements its own behavior for each method.
 * IGameState contains methods for each state to use such as Render(), HandleInput() and Update().
 * 
 * Lifecycle:
 *   1. State is created
 *   2. OnEnter() is called when state becomes active
 *   3. HandleInput() / Update() / Render() called each frame
 *   4. OnExit() is called before transitioning to another state
 *   5. State is destroyed
 */
class IGameState {
public:

    virtual ~IGameState() = default;

    /**
     * @brief Called once when this state becomes the active state.
     * Use for initialization: loading assets, resetting variables,
     * displaying welcome messages, etc.
     */
    virtual void OnEnter(GameContext& context) = 0;

    /**
     * @brief Called once when leaving this state for another.
     * Use for cleanup: saving progress, releasing resources, etc.
     */
    virtual void OnExit(GameContext& context) = 0;

    /**
     * @brief Process user input specific to this state.
     * In MainMenu: handle class selection
     * In Combat: handle attack/ability selection
     * In Shop: handle buy/sell choices
     */
    virtual void HandleInput(GameContext& context) = 0;

    /**
     * @brief Update game logic for this state.
     * Could process AI, physics, timers, etc.
     * For a text-based RPG, this might be minimal.
     */
    virtual void Update(GameContext& context) = 0;

    /**
     * @brief Render/display output for this state.
     * For console: print menus, status, messages.
     * For graphics: draw sprites, UI, etc.
     */
    virtual void Render(GameContext& context) = 0;
};

#endif // IGAMESTATE_H
