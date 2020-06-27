cmake_minimum_required(VERSION 3.5)

add_library(Nwn STATIC
    src/alignment.cpp
    src/character.cpp
    src/race.cpp
    src/rules.cpp
    src/ablblock.cpp
    src/chclass.cpp
    )

target_include_directories(Nwn PUBLIC include)

target_link_libraries(Nwn PUBLIC
    Boost::boost
    Boost::serialization
    )
