#pragma once

// The StatBlock class holds the core statistics for any character or monster.
class StatBlock {
public:
    StatBlock(int s, int i, int a, int arm, int res);

    int GetStrength();
    int GetIntellect();
    int GetAgility();
    int GetArmor();
    int GetElementRes();

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
};
