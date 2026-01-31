#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Item.h"
#include "Potion.h"
// Add more item includes here as you create them (e.g., Sword.h)

// Justification: Factory Pattern
// The Factory Pattern centralizes object creation.
// Instead of scattering `new Potion(...)` everywhere, we call `ItemFactory::Create(...)`.
// This makes it easy to add new item types without changing the calling code.

// ShopItem struct: Pairs an item with its base price for shop display.
struct ShopItem {
    std::string name;
    int basePrice;

    // Factory method to create the actual item
    std::unique_ptr<Item> CreateInstance() const;
};

class ItemFactory {
public:
    // Justification: Static Method
    // We don't need an instance of ItemFactory to create items.
    // Using a static method makes it globally accessible.
    static std::unique_ptr<Item> CreateItem(const std::string& itemName);

    // Returns the catalog of items available for purchase (name + base price).
    static std::vector<ShopItem> GetShopCatalog();
};
