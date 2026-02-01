#pragma once

// The StatBlock class holds the core statistics for any character or monster.
class StatBlock {
public:
    // Constructor: STR, INT, AGI, ARM, RES, MaxRP
    StatBlock(int s, int i, int a, int arm, int res, int maxRP = 100);

    // Getters
    int GetStrength() const;
    int GetIntellect() const;
    int GetAgility() const;
    int GetArmor() const;
    int GetElementRes() const;
    int GetMaxResourcePoints() const;

    // Methods to increase stats (needed for Leveling)
    void IncreaseStats(int s, int i, int a, int arm, int res);

    // Operator overloading to easily manipulate stats
    StatBlock operator+(const StatBlock& other) const;
    StatBlock operator-(const StatBlock& other) const;
    StatBlock& operator+=(const StatBlock& other);
    StatBlock& operator-=(const StatBlock& other);

private:
    int m_Strength;
    int m_Intellect;
    int m_Agility;
    int m_Armor;
    int m_ElementRes;
    int m_MaxResourcePoints;  // Max RP for special abilities
};
