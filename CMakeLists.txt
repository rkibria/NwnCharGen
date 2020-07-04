cmake_minimum_required(VERSION 3.5)

project(NwnCharGen LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

# set(Boost_DEBUG ON)
if(WIN32)
    set(Boost_COMPILER "-mgw8")
endif()
set(Boost_ARCHITECTURE "-x64")
find_package(Boost REQUIRED COMPONENTS serialization)

add_subdirectory(Nwn)

qt5_wrap_cpp(MOC_SOURCES
    nwnchargen.h
    racedialog.h
    raceeditdialog.h
    levelsmodel.h
    levelsclassdelegate.h
    levelsabldelegate.h
    chclassdialog.h
    )

qt5_wrap_ui(UI_SOURCES
    nwnchargen.ui
    racedialog.ui
    raceeditdialog.ui
    chclassdialog.ui
    )

qt5_add_resources(QRC_SOURCES
    qdarkstyle/style.qrc
    )

add_executable(NwnCharGen
    main.cpp
    nwnchargen.cpp
    racedialog.cpp
    raceeditdialog.cpp
    levelsmodel.cpp
    levelsclassdelegate.cpp
    chclassdialog.cpp
    levelsabldelegate.cpp

    ${MOC_SOURCES}
    ${UI_SOURCES}
    ${QRC_SOURCES}
    )

unset(MOC_SOURCES)
unset(UI_SOURCES)
unset(QRC_SOURCES)

target_link_libraries(NwnCharGen PRIVATE
    Qt5::Widgets
    Nwn
    )
