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

private:
    int m_Strength;
    int m_Intellect;
    int m_Agility;
    int m_Armor;
    int m_ElementRes;
};
