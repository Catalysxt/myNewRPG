#include "Item.h"

Item::Item(std::string name, std::string description)
    : m_Name(name), m_Description(description) {
}

std::string Item::GetName() {
    return m_Name;
}

std::string Item::GetDescription() {
    return m_Description;
}
