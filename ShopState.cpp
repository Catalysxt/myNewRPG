#include "ShopState.h"
#include "GameContext.h"
#include "Character.h"
#include "Merchant.h"
#include "TradeResult.h"
#include <iostream>

void ShopState::OnEnter(GameContext& context) {
    m_ExitRequested = false;
    
    std::cout << "\n----------------------------------------" << std::endl;
    std::cout << "You encounter " << context.merchant->GetName() << "!" << std::endl;
    std::cout << "\"Welcome, traveler! Care to browse my wares?\"" << std::endl;
}

void ShopState::OnExit(GameContext& context) {
    std::cout << "\"Come back anytime!\" says " << context.merchant->GetName() << std::endl;
}

void ShopState::HandleInput(GameContext& context) {
    if (m_ExitRequested) {
        return;
    }

    std::cout << "\nYour Gold: " << context.player->GetGold() << std::endl;
    context.merchant->DisplayStock();
    std::cout << "0. Leave Shop" << std::endl;
    std::cout << "Enter item # to buy (0 to exit): ";
    
    int choice;
    std::cin >> choice;
    
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid input!" << std::endl;
        return;
    }
    
    if (choice == 0) {
        m_ExitRequested = true;
    } else {
        // Attempt to buy item (0-indexed internally)
        TradeResult result = context.merchant->SellItem(*context.player, choice - 1);
        
        switch (result) {
            case TradeResult::Success:
                // Message already printed by Merchant
                break;
            case TradeResult::InsufficientGold:
                // Message already printed by SpendGold
                break;
            case TradeResult::ItemNotFound:
                std::cout << "That item is not available." << std::endl;
                break;
            case TradeResult::InventoryFull:
                std::cout << "Your inventory is full!" << std::endl;
                break;
        }
    }
}

void ShopState::Update(GameContext& context) {
    if (m_ExitRequested) {
        context.RequestStateChange(StateType::Training);
    }
}

void ShopState::Render(GameContext& context) {
    // Console rendering handled in HandleInput
}
