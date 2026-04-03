#include "Potion.h"
#include "Character.h" // Now we need the full definition to access methods like GetCurrentHP

Potion::Potion(int healAmount)
    : Item("Health Potion", "Restores HP"), m_HealAmount(healAmount) {
}

void Potion::Use(Character* user) {
    // We haven't implemented a clean "Heal" function in Character yet,
    // so let's check one isn't needed or simply manipulate variables if friends (likely not).
    // Better practice: Add a Heal() method to Character.
    // For now, I will use a hypothetical Heal() method and add it to Character next.
    
    std::cout << "You drink the potion..." << std::endl;
    user->Heal(m_HealAmount); // We can call Character methods
}
