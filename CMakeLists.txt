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
    src/main.cpp

    src/character/StatBlock.cpp
    src/character/Character.cpp
    src/character/Warrior.cpp
    src/character/Wizard.cpp
    src/character/Cleric.cpp
    src/character/Rogue.cpp

    src/monsters/Monster.cpp
    src/monsters/Orc.cpp
    src/monsters/Slime.cpp
    src/monsters/Goblin.cpp

    src/item/Item.cpp
    src/item/Potion.cpp
    src/item/SlimeBubble.cpp
    src/item/IronHammer.cpp
    
    src/item/ItemFactory.cpp
    src/economy/Merchant.cpp
    src/combatEngine/CombatEngine.cpp
    src/monsters/MonsterFactory.cpp
    src/events/EventBus.cpp

    # Game State Machine files
    src/states/GameStateManager.cpp
    src/states/MainMenuState.cpp
    src/states/ShopState.cpp
    src/states/TrainingAreaState.cpp
    src/states/CombatState.cpp
    src/states/GameOverState.cpp
)

# Include directories so headers in subfolders are found by the compiler
target_include_directories(myRPG PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/character
    ${CMAKE_SOURCE_DIR}/src/item
    ${CMAKE_SOURCE_DIR}/src/monsters
    ${CMAKE_SOURCE_DIR}/src/states
    ${CMAKE_SOURCE_DIR}/src/combatEngine
    ${CMAKE_SOURCE_DIR}/src/strategies
    ${CMAKE_SOURCE_DIR}/src/events
    ${CMAKE_SOURCE_DIR}/src/economy
)
