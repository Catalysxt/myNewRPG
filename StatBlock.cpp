#include "StatBlock.h"

StatBlock::StatBlock(int s, int i, int a, int arm, int res, int maxRP)
    : m_Strength(s), m_Intellect(i), m_Agility(a), m_Armor(arm), 
      m_ElementRes(res), m_MaxResourcePoints(maxRP) {
}

int StatBlock::GetStrength() const { return m_Strength; }

int StatBlock::GetIntellect() const { return m_Intellect; }

int StatBlock::GetAgility() const { return m_Agility; }

int StatBlock::GetArmor() const { return m_Armor; }

int StatBlock::GetElementRes() const { return m_ElementRes; }

int StatBlock::GetMaxResourcePoints() const { return m_MaxResourcePoints; }


void StatBlock::IncreaseStats(int s, int i, int a, int arm, int res) {
    m_Strength += s;
    m_Intellect += i;
    m_Agility += a;
    m_Armor += arm;
    m_ElementRes += res;
}

// Returns a new StatBlock with combined stats
StatBlock StatBlock::operator+(const StatBlock& other) const {
    return StatBlock(
        m_Strength + other.m_Strength,
        m_Intellect + other.m_Intellect,
        m_Agility + other.m_Agility,
        m_Armor + other.m_Armor,
        m_ElementRes + other.m_ElementRes
    );
}

// Returns a new StatBlock with subtracted stats
StatBlock StatBlock::operator-(const StatBlock& other) const {
    return StatBlock(
        m_Strength - other.m_Strength,
        m_Intellect - other.m_Intellect,
        m_Agility - other.m_Agility,
        m_Armor - other.m_Armor,
        m_ElementRes - other.m_ElementRes
    );
}

// Adds stats in place and returns reference to self
StatBlock& StatBlock::operator+=(const StatBlock& other) {
    m_Strength += other.m_Strength;
    m_Intellect += other.m_Intellect;
    m_Agility += other.m_Agility;
    m_Armor += other.m_Armor;
    m_ElementRes += other.m_ElementRes;
    return *this;
}

// Subtracts stats in place and returns reference to self
StatBlock& StatBlock::operator-=(const StatBlock& other) {
    m_Strength -= other.m_Strength;
    m_Intellect -= other.m_Intellect;
    m_Agility -= other.m_Agility;
    m_Armor -= other.m_Armor;
    m_ElementRes -= other.m_ElementRes;
    return *this;
}
