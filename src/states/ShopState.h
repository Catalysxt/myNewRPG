#ifndef SHOPSTATE_H
#define SHOPSTATE_H

#include "IGameState.h"

/**
 * @brief State for merchant interaction and shopping.
 * 
 * Handles:
 *   - Displaying merchant inventory
 *   - Buying items
 *   - Viewing player inventory/gold
 *   - Transition to TrainingAreaState
 */
class ShopState : public IGameState {
public:
    ShopState() = default;
    ~ShopState() override = default;

    void OnEnter(GameContext& context) override;
    void OnExit(GameContext& context) override;
    void HandleInput(GameContext& context) override;
    void Update(GameContext& context) override;
    void Render(GameContext& context) override;

private:
    bool m_ExitRequested = false;
};

#endif // SHOPSTATE_H
