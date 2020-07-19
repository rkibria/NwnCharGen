cmake_minimum_required(VERSION 3.12)

add_library(Nwn STATIC
    src/abilities.cpp
    src/ablblock.cpp
    src/alignment.cpp
    src/character.cpp
    src/chclass.cpp
    src/dice.cpp
    src/race.cpp
    src/rules.cpp
    )

target_include_directories(Nwn PUBLIC include)

target_link_libraries(Nwn PUBLIC
    Boost::boost
    Boost::serialization
    )
