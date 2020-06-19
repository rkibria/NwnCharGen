cmake_minimum_required(VERSION 3.5)

project(NwnCharGen LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
# find_package(Boost 1.70 REQUIRED)

add_library(Nwn STATIC
    nwnchar.cpp
    nwnrace.cpp
    nwnrules.cpp
    )

qt5_wrap_cpp(MOC_SOURCES
    nwnchargen.h
    racedialog.h
    )

qt5_wrap_ui(UI_SOURCES
    nwnchargen.ui
    racedialog.ui
    )

add_executable(NwnCharGen
    main.cpp
    nwnchargen.cpp
    racedialog.cpp
    ${MOC_SOURCES}
    ${UI_SOURCES}
    )

unset(MOC_SOURCES)
unset(UI_SOURCES)

target_link_libraries(NwnCharGen PRIVATE
    Qt5::Widgets
    Nwn
    )
