#include <vector>
#include <string_view>
#include <unordered_map>
#include <functional>

#include "ItemFactory.h"

// Justification: Factory Pattern Implementation
// This is where we define HOW items are created based on their name.

std::unordered_map<std::string, std::function<std::unique_ptr<Item>()>>& ItemFactory::GetRegistry() {
    static std::unordered_map<std::string, std::function<std::unique_ptr<Item>()>> m_Registry;
    return m_Registry; 
}


void ItemFactory::Register(const std::string& name, std::function<std::unique_ptr<Item>()> factory_fn) {
    GetRegistry()[name] = std::move(factory_fn);
}

std::unique_ptr<Item> ShopItem::CreateInstance() const {
    return ItemFactory::CreateItem(name);
}

std::unique_ptr<Item> ItemFactory::CreateItem(std::string_view itemName) {
    auto& registry = GetRegistry();

    // Convert parameter into string so we can lookup in the map
    auto it = registry.find(std::string(itemName));
    if (it != registry.end()) {
        // Found a match. Create the item
        return it->second();
    }
    
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
