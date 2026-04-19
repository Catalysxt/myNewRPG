#include <vector>
#include <string_view>

#include "ItemFactory.h"


// Justification: Factory Pattern Implementation
// This is where we define HOW items are created based on their name.

std::unique_ptr<Item> ShopItem::CreateInstance() const {
    return ItemFactory::CreateItem(name);
}

std::unique_ptr<Item> ItemFactory::CreateItem(std::string_view itemName) {
    // Justification: Centralized Creation Logic
    // All item instantiation happens here. To add a new item type,
    // just add a new `else if` block.
    if (itemName == "Health Potion") {
        return std::make_unique<Potion>(30);
    } 
    else if (itemName == "Greater Health Potion") {
        return std::make_unique<Potion>(75);
    }
    // Add more items here as you create them:
    // else if (itemName == "Iron Sword") {
    //     return std::make_unique<Sword>(10); // Damage: 10
    // }
    
    return nullptr; // Item not found
}

std::vector<ShopItem> ItemFactory::GetShopCatalog() {
    // Justification: Single Source of Truth for Shop Inventory
    // This defines what items are available and their base prices.
    // The purchasing of items is separate from the creation of items.
    return {
        {"Health Potion", 25},
        {"Greater Health Potion", 60}
        // Add more items here
    };
}
