#ifndef COMBATSTATE_H
#define COMBATSTATE_H

#include "IGameState.h"
#include "Monster.h"
#include <memory>

/**
 * @brief State for combat encounters.
 * 
 * Handles the complete battle system:
 *   - Turn-based combat (player turn / enemy turn)
 *   - Attack, special abilities, items
 *   - Combat resolution (victory/defeat)
 *   - Loot and XP rewards
 * 
 * Uses an internal BattlePhase enum to manage combat flow,
 * separate from the game-wide state machine.
 */
class CombatState : public IGameState {
public:
    CombatState() = default;
    ~CombatState() override = default;

    void OnEnter(GameContext& context) override;
    void OnExit(GameContext& context) override;
    void HandleInput(GameContext& context) override;
    void Update(GameContext& context) override;
    void Render(GameContext& context) override;

private:
    // Internal combat phases (not game states!)
    enum class BattlePhase {
        PlayerTurn,
        EnemyTurn,
        Victory,
        Defeat
    };

    void HandlePlayerTurn(GameContext& context);
    void HandleEnemyTurn(GameContext& context);
    void HandleVictory(GameContext& context);
    void HandleDefeat(GameContext& context);

    std::unique_ptr<Monster> m_CurrentEnemy;
    BattlePhase m_Phase = BattlePhase::PlayerTurn;
    bool m_BattleEnded = false;
    bool m_IsTraining = false;
};

#endif // COMBATSTATE_H
