#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include "IGameState.h"
#include "GameContext.h"

/**
 * @brief State displayed after combat ends.
 * 
 * Shows victory or defeat message based on GameContext::playerWon.
 * Provides options to:
 *   - Continue (return to training area after victory)
 *   - Play Again (restart from MainMenu)
 *   - Quit
 */
class GameOverState : public IGameState {
public:
    GameOverState() = default;
    ~GameOverState() override = default;

    void OnEnter(GameContext& context) override;
    void OnExit(GameContext& context) override;
    void HandleInput(GameContext& context) override;
    void Update(GameContext& context) override;
    void Render(GameContext& context) override;

private:
    StateType m_NextState = StateType::None;
    bool m_PlayerWon = false;
};

#endif // GAMEOVERSTATE_H
