#include "CombatState.h"
#include "GameContext.h"
#include "Character.h"
#include "Monster.h"
#include "MonsterFactory.h"
#include "CombatEngine.h"
#include "Merchant.h"
#include "TradeResult.h"
#include "Slime.h"
#include <iostream>

void CombatState::OnEnter(GameContext& context) {
    m_Phase = BattlePhase::PlayerTurn;
    m_BattleEnded = false;
    m_IsTraining = context.isTrainingCombat;
    
    // Spawn enemy based on combat type
    if (m_IsTraining) {
        // Training dummy is a weak slime
        m_CurrentEnemy = std::make_unique<Slime>();
        std::cout << "\n--- TRAINING BATTLE ---" << std::endl;
        std::cout << "A training dummy appears! (No death penalty)" << std::endl;
    } else {
        // Real combat - spawn random monster
        m_CurrentEnemy = context.monsterFactory->SpawnRandom();
        std::cout << "\n--- BATTLE START ---" << std::endl;
    }
    
    std::cout << context.player->GetName() << " VS " << m_CurrentEnemy->GetClassName() << "!" << std::endl;
    std::cout << "Enemy HP: " << m_CurrentEnemy->GetMaxHP() << std::endl;
}

void CombatState::OnExit(GameContext& context) {
    // Clean up enemy
    m_CurrentEnemy.reset();
}

void CombatState::HandleInput(GameContext& context) {
    if (m_BattleEnded) {
        return;
    }

    switch (m_Phase) {
        case BattlePhase::PlayerTurn:
            HandlePlayerTurn(context);
            break;
        case BattlePhase::EnemyTurn:
            HandleEnemyTurn(context);
            break;
        case BattlePhase::Victory:
            HandleVictory(context);
            break;
        case BattlePhase::Defeat:
            HandleDefeat(context);
            break;
    }
}

void CombatState::Update(GameContext& context) {
    if (m_BattleEnded) {
        if (m_IsTraining) {
            // Return to training area after practice
            context.RequestStateChange(StateType::Training);
        } else {
            // Go to game over for real combat
            context.RequestStateChange(StateType::GameOver);
        }
    }
}

void CombatState::Render(GameContext& context) {
    // Console rendering handled in HandleInput
}

void CombatState::HandlePlayerTurn(GameContext& context) {
    std::cout << "\n=======================================================" << std::endl;
    std::cout << "[Your Turn] HP: " << context.player->GetCurrentHP() << "/" << context.player->GetMaxHP()
              << "  |  RP: " << context.player->GetCurrentRP() << "/" << context.player->GetMaxRP() << std::endl;
    std::cout << "1. Attack" << std::endl;
    std::cout << "2. " << context.player->GetAbilityName() 
              << " (" << context.player->GetAbilityCost() << " RP)" << std::endl;
    std::cout << "3. Inventory" << std::endl;
    std::cout << "4. Shop" << std::endl;
    std::cout << "=======================================================" << std::endl;
    std::cout << "Choose: ";
    
    int action;
    if (!(std::cin >> action)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        action = 0;
    }

    // ATTACK LOGIC
    if (action == 1) {
        AttackResult result = context.combatEngine->CalculateAttack(
            context.player->GetStats(),
            m_CurrentEnemy->GetStats(),
            context.player->GetDamageStrategy(),
            context.player->GetBaseDamage()
        );
        
        if (result.wasDodged) {
            std::cout << "The " << m_CurrentEnemy->GetClassName() << " dodges your attack!" << std::endl;
        } else {
            std::cout << "You attack the " << m_CurrentEnemy->GetClassName();
            if (result.isCritical) {
                std::cout << " with a CRITICAL HIT";
            }
            std::cout << " for " << result.finalDamage << " damage!" << std::endl;
            m_CurrentEnemy->TakeDamage(result.finalDamage);
            std::cout << "Enemy HP: " << m_CurrentEnemy->GetCurrentHP() << "/" << m_CurrentEnemy->GetMaxHP() << std::endl;
        }

        if (m_CurrentEnemy->GetCurrentHP() <= 0) {
            m_Phase = BattlePhase::Victory;
        } else {
            m_Phase = BattlePhase::EnemyTurn;
        }
    }
    
    // SPECIAL ABILITY
    else if (action == 2) {
        context.player->PerformSpecialAbility(m_CurrentEnemy.get(), *context.combatEngine);
        
        if (m_CurrentEnemy->GetCurrentHP() <= 0) {
            m_Phase = BattlePhase::Victory;
        } else {
            m_Phase = BattlePhase::EnemyTurn;
        }
    }
    
    // INVENTORY LOGIC
    else if (action == 3) {
        if (context.player->HasItems()) {
            context.player->PrintInventory();
            std::cout << "0. Back" << std::endl;
            std::cout << "Enter item # to use: ";
            
            int itemIndex;
            std::cin >> itemIndex;
            
            if (itemIndex > 0 && itemIndex <= 100) {
                context.player->UseItem(itemIndex - 1);
                m_Phase = BattlePhase::EnemyTurn;
            } else {
                std::cout << "Cancelled." << std::endl;
            }
        } else {
            std::cout << "\nYou have no items!" << std::endl;
        }
    }
    
    // SHOP LOGIC (mid-battle shopping!)
    else if (action == 4) {
        bool shopping = true;
        while (shopping) {
            std::cout << "\nYour Gold: " << context.player->GetGold() << std::endl;
            context.merchant->DisplayStock();
            std::cout << "Enter item # to buy (0 to exit): ";
            
            int choice;
            std::cin >> choice;
            
            if (choice == 0) {
                shopping = false;
            } else {
                TradeResult result = context.merchant->SellItem(*context.player, choice - 1);
                switch (result) {
                    case TradeResult::Success: break;
                    case TradeResult::InsufficientGold: break;
                    case TradeResult::ItemNotFound:
                        std::cout << "That item is not available." << std::endl;
                        break;
                    case TradeResult::InventoryFull:
                        std::cout << "Your inventory is full!" << std::endl;
                        break;
                }
            }
        }
        std::cout << "You return to the battle." << std::endl;
        // Shopping doesn't end turn
    }
    
    else {
        std::cout << "Invalid action! You stumble." << std::endl;
        m_Phase = BattlePhase::EnemyTurn;
    }
}

void CombatState::HandleEnemyTurn(GameContext& context) {
    std::cout << "\n=======================================================" << std::endl;
    std::cout << "[Enemy Turn]" << std::endl;
    std::cout << "=======================================================" << std::endl;
    
    // Check if enemy is STUNNED
    if (m_CurrentEnemy->HasStatus(StatusEffect::Stunned)) {
        std::cout << "The " << m_CurrentEnemy->GetClassName() << " is STUNNED and cannot act!" << std::endl;
        m_CurrentEnemy->RemoveStatus(StatusEffect::Stunned);
        m_Phase = BattlePhase::PlayerTurn;
        return;
    }
    
    // Enemy attack
    AttackResult result = context.combatEngine->CalculateAttack(
        m_CurrentEnemy->GetStats(),
        context.player->GetStats(),
        m_CurrentEnemy->GetDamageStrategy(),
        m_CurrentEnemy->GetBaseDamage()
    );
    
    if (result.wasDodged) {
        std::cout << "You dodge the " << m_CurrentEnemy->GetClassName() << "'s attack!" << std::endl;
    } else {
        std::cout << "The " << m_CurrentEnemy->GetClassName() << " attacks you";
        if (result.isCritical) {
            std::cout << " with a CRITICAL HIT";
        }
        std::cout << " for " << result.finalDamage << " damage!" << std::endl;
        context.player->TakeDamage(result.finalDamage);
    }

    if (context.player->GetCurrentHP() <= 0) {
        m_Phase = BattlePhase::Defeat;
    } else {
        m_Phase = BattlePhase::PlayerTurn;
    }
}

void CombatState::HandleVictory(GameContext& context) {
    std::cout << "\nVICTORY! You defeated the " << m_CurrentEnemy->GetClassName() << "!" << std::endl;
    
    if (!m_IsTraining) {
        // Award XP
        context.player->GainXP(50);
        
        // Award gold
        int goldDrop = m_CurrentEnemy->GetGoldDrop(*context.combatEngine);
        context.player->AddGold(goldDrop);
        std::cout << "You found " << goldDrop << " gold!" << std::endl;
        
        // Check for loot
        auto loot = m_CurrentEnemy->GetLootDrop(*context.combatEngine);
        if (loot) {
            std::cout << "The " << m_CurrentEnemy->GetClassName() << " dropped: " << loot->GetName() << "!" << std::endl;
            context.player->AddItem(std::move(loot));
        }
    } else {
        std::cout << "Good practice! You feel more confident." << std::endl;
    }
    
    context.playerWon = true;
    m_BattleEnded = true;
}

void CombatState::HandleDefeat(GameContext& context) {
    if (m_IsTraining) {
        std::cout << "\nYou were knocked out during training." << std::endl;
        std::cout << "A healer revives you at the training grounds." << std::endl;
        // Restore some HP for training
        context.player->Heal(context.player->GetMaxHP() / 2);
        context.playerWon = false;
        m_BattleEnded = true;
    } else {
        std::cout << "\nDEFEAT! You have fallen..." << std::endl;
        context.playerWon = false;
        m_BattleEnded = true;
    }
}
