cmake_minimum_required(VERSION 3.5)

add_library(Nwn STATIC
    src/nwnchar.cpp
    src/nwnrace.cpp
    src/nwnrules.cpp
    src/nwnablblock.cpp
    )

target_include_directories(Nwn PUBLIC include)
