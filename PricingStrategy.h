#pragma once
#include <string>

// Justification: Strategy Pattern
// The Strategy Pattern defines a family of algorithms (pricing strategies),
// encapsulates each one, and makes them interchangeable.
// This allows us to implement different pricing logic for the Merchant

class PricingStrategy {
public:
    virtual ~PricingStrategy() = default;

    // Justification: Pure Virtual Function
    // Every concrete strategy MUST implement CalculatePrice.
    // The base price is passed in, and the strategy returns the final price.
    virtual int CalculatePrice(int basePrice) const = 0;

    // Justification: Const Correctness
    // GetName() doesn't modify the object, so it's marked const.
    virtual std::string GetName() const = 0;
};

// --- Concrete Strategies ---

class StandardPricing : public PricingStrategy {
public:
    int CalculatePrice(int basePrice) const override {
        return basePrice; // No markup
    }
    std::string GetName() const override { return "Standard Pricing"; }
};

class GreedyMerchantPricing : public PricingStrategy {
public:
    // Justification: Encapsulation
    // The markup factor is internal to the strategy.
    int CalculatePrice(int basePrice) const override {
        return static_cast<int>(basePrice * 1.5); // 50% markup
    }
    std::string GetName() const override { return "Greedy Merchant (150%)"; }
};

class FriendlyMerchantPricing : public PricingStrategy {
public:
    int CalculatePrice(int basePrice) const override {
        return static_cast<int>(basePrice * 0.8); // 20% discount
    }
    std::string GetName() const override { return "Friendly Merchant (80%)"; }
};

// --- Advanced Strategy: Class-Based Pricing ---
// This strategy holds state (the buyer's class name).
// Call SetBuyer() BEFORE CalculatePrice() to configure it.

class WarriorGuildPricing : public PricingStrategy {
private:
    std::string m_BuyerClassName;
    
public:
    // Configure who is buying before price calculation
    void SetBuyer(const std::string& className) {
        m_BuyerClassName = className;
    }
    
    int CalculatePrice(int basePrice) const override {
        // Warriors get 30% off, everyone else pays full price
        if (m_BuyerClassName == "Warrior") {
            return static_cast<int>(basePrice * 0.7);
        }
        return basePrice;
    }
    
    std::string GetName() const override { 
        return "Warrior Guild (70% off for Warriors)"; 
    }
};

// The context (The Merchant) is coded in a separate file. The context (step 3 of the strategy plan) provides an interface
// to interact with pricing strategies 
