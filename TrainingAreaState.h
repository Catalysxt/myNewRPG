#ifndef TRAININGAREASTATSE_H
#define TRAININGAREASTATSE_H

#include "IGameState.h"
#include "GameContext.h"

/**
 * @brief Hub state for the training area.
 * 
 * This is the central hub where the player chooses their next activity:
 *   - Fight training dummy (practice combat)
 *   - Enter dungeon (real combat)
 *   - Visit shop
 *   - View stats
 */
class TrainingAreaState : public IGameState {
public:
    TrainingAreaState() = default;
    ~TrainingAreaState() override = default;

    void OnEnter(GameContext& context) override;
    void OnExit(GameContext& context) override;
    void HandleInput(GameContext& context) override;
    void Update(GameContext& context) override;
    void Render(GameContext& context) override;

private:
    StateType m_NextState = StateType::None;
};

#endif // TRAININGAREASTATSE_H
