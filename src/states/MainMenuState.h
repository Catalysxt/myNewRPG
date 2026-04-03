#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "IGameState.h"
#include <string>

/**
 * @brief Initial game state for character creation.
 * 
 * Handles:
 *   - Class selection (Warrior, Wizard, Cleric, Rogue)
 *   - Name input
 *   - Character creation via factory
 *   - Transition to ShopState
 */
class MainMenuState : public IGameState {
public:
    MainMenuState() = default;
    ~MainMenuState() override = default;

    void OnEnter(GameContext& context) override;
    void OnExit(GameContext& context) override;
    void HandleInput(GameContext& context) override;
    void Update(GameContext& context) override;
    void Render(GameContext& context) override;

private:
    bool m_CharacterCreated = false;
};

#endif // MAINMENUSTATE_H
