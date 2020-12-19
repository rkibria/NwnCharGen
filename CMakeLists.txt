cmake_minimum_required(VERSION 3.12)

project(NwnCharGen LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

# set(Boost_DEBUG ON)
if(WIN32)
    set(Boost_COMPILER "-mgw8")
endif()
set(Boost_ARCHITECTURE "-x64")
find_package(Boost REQUIRED COMPONENTS
    serialization
    filesystem
    )

# work around "cc1plus.exe: out of memory"
# https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56746
add_compile_options(-save-temps -ftrack-macro-expansion=0)

add_subdirectory(Nwn)

set( NWN2DATALIB_PATH "" CACHE STRING "Path to NWN2DataLib sources" )
set( NWN2_PATH "" CACHE STRING "Path to the game installation" )

if( IS_DIRECTORY "${NWN2DATALIB_PATH}" AND IS_DIRECTORY "${NWN2_PATH}" )
    message( STATUS "Adding rulesgen (NWN2DataLib=${NWN2DATALIB_PATH} NWN2=${NWN2_PATH})" )
    add_subdirectory( "${NWN2DATALIB_PATH}" binary_NWN2DataLib )
    add_subdirectory( unziphelper )
    add_subdirectory( rulesgen )
else()
    message( STATUS "Not adding rulesgen" )
endif()

qt5_wrap_cpp(MOC_SOURCES
    nwnchargen.h
    racedialog.h
    raceeditdialog.h
    levelsmodel.h
    levelsclassdelegate.h
    levelsabldelegate.h
    chclassdialog.h
    featchoicedelegate.h
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
    featchoicedelegate.cpp

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
