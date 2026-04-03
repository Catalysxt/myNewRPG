#include "Merchant.h"

Merchant::Merchant(const std::string& name, std::unique_ptr<PricingStrategy> pricingStrategy)
    : m_Name(name), m_PricingStrategy(std::move(pricingStrategy)) {
    // Justification: RAII (Resource Acquisition Is Initialization)
    // We initialize the stock immediately in the constructor.
    // The ItemFactory provides the catalog.
    m_Stock = ItemFactory::GetShopCatalog();
}

std::string Merchant::GetName() const {
    return m_Name;
}

void Merchant::DisplayStock() const {
    // Justification: Const Correctness
    // This method only reads data, so it's marked const.
    std::cout << "\n--- " << m_Name << "'s Shop ---" << std::endl;
    std::cout << "Pricing: " << m_PricingStrategy->GetName() << std::endl;
    // This is call the derived classes of m_PricingStrategy    
    std::cout << "----------------------------" << std::endl;

    for (size_t i = 0; i < m_Stock.size(); ++i) {
        int finalPrice = m_PricingStrategy->CalculatePrice(m_Stock[i].basePrice); // Price calculation
        std::cout << (i + 1) << ". " << m_Stock[i].name 
                  << " - " << finalPrice << " gold" << std::endl;
    }
    std::cout << "0. Exit Shop" << std::endl;
    std::cout << "----------------------------" << std::endl;
}

TradeResult Merchant::SellItem(Character& buyer, int itemIndex) {

    // Validate the selection before accessing the vector to prevent undefined behavior.
    if (itemIndex < 0 || itemIndex >= static_cast<int>(m_Stock.size())) {
        return TradeResult::ItemNotFound;
    }

    const ShopItem& item = m_Stock[itemIndex];
    int finalPrice = m_PricingStrategy->CalculatePrice(item.basePrice);

    // Justification: Encapsulation
    // We use the Character's SpendGold() method instead of directly accessing m_Gold.
    // This keeps the logic for "can I afford this?" inside the Character class.
    if (!buyer.SpendGold(finalPrice)) {
        return TradeResult::InsufficientGold;
    }

    // Justification: Factory Pattern
    // We use the ShopItem's CreateInstance() method (which calls ItemFactory)
    // to generate a fresh item instance for the buyer.
    std::unique_ptr<Item> newItem = item.CreateInstance();
    
    if (!newItem) {
        // Something went wrong in the factory. Refund the gold.
        buyer.AddGold(finalPrice);
        return TradeResult::ItemNotFound;
    }

    // Justification: RAII / Move Semantics
    // We std::move() the unique_ptr into the buyer's inventory.
    // Ownership is transferred, and we don't need to worry about memory leaks.
    buyer.AddItem(std::move(newItem));
    
    std::cout << buyer.GetName() << " purchased " << item.name 
              << " for " << finalPrice << " gold." << std::endl;

    return TradeResult::Success;
}
