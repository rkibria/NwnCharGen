cmake_minimum_required(VERSION 3.5)

add_library(Nwn STATIC
    src/char.cpp
    src/race.cpp
    src/rules.cpp
    src/ablblock.cpp
    )

target_include_directories(Nwn PUBLIC include)
