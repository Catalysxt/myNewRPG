#include "StatBlock.h"

StatBlock::StatBlock(int s, int i, int a, int arm, int res)
    : m_Strength(s), m_Intellect(i), m_Agility(a), m_Armor(arm), m_ElementRes(res) {
}

int StatBlock::GetStrength() { return m_Strength; }
int StatBlock::GetIntellect() { return m_Intellect; }
int StatBlock::GetAgility() { return m_Agility; }
int StatBlock::GetArmor() { return m_Armor; }
int StatBlock::GetElementRes() { return m_ElementRes; }

void StatBlock::IncreaseStats(int s, int i, int a, int arm, int res) {
    m_Strength += s;
    m_Intellect += i;
    m_Agility += a;
    m_Armor += arm;
    m_ElementRes += res;
}
