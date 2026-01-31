#pragma once
#include <memory>
#include <vector>
#include <string>
#include <iostream>

#include "TradeResult.h"
#include "PricingStrategy.h"
#include "ItemFactory.h"
#include "Character.h"

// Justification: Separation of Concerns
// The Merchant class is responsible ONLY for managing shop stock and transactions.
// It delegates pricing logic to a PricingStrategy and item creation to ItemFactory.

class Merchant {
public:
    // Justification: Dependency Injection via Constructor
    // The PricingStrategy is passed in, allowing different merchants to have
    // different pricing behaviors without modifying the Merchant class itself.
    Merchant(const std::string& name, std::unique_ptr<PricingStrategy> pricingStrategy);
                                    
    // Justification: Rule of Five (Implicit)
    // Since we use unique_ptr, the default destructor handles cleanup (RAII).
    ~Merchant() = default;

    // Justification: Reference Passing
    // We pass the Character by REFERENCE (&). We don't want to operate on duplicates of the Character's items
    TradeResult SellItem(Character& buyer, int itemIndex);

    // UI Methods
    void DisplayStock() const;
    std::string GetName() const;

private:
    std::string m_Name;
    std::unique_ptr<PricingStrategy> m_PricingStrategy;
    std::vector<ShopItem> m_Stock; // Items available for sale. ShopItem is a struct
};
