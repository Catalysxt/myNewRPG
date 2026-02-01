cmake_minimum_required(VERSION 3.10)

# Project Name
project(MyNewRPG VERSION 1.0)

# Use C++17 Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add all source files explicitly
# This is better practice than using wildcards (GLOB) because CMake 
# will know to re-build if you add/remove files from this list.
add_executable(myRPG
    main.cpp
    StatBlock.cpp
    Character.cpp
    Warrior.cpp
    Wizard.cpp
    Cleric.cpp
    Rogue.cpp
    Monster.cpp
    Orc.cpp
    Slime.cpp
    Item.cpp
    Potion.cpp
    ItemFactory.cpp
    Merchant.cpp
    CombatEngine.cpp
)
