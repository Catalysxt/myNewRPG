#pragma once

// Justification: Enum Class for Type Safety
// Using enum class instead of plain enum prevents implicit conversion to int
// and provides a scoped namespace for the values.
enum class TradeResult {
    Success,           // Transaction completed successfully
    InsufficientGold,  // Buyer doesn't have enough gold
    ItemNotFound,      // Requested item is not in the shop stock
    InventoryFull      // Buyer's inventory is full (not implemented yet, but reserved)
};
