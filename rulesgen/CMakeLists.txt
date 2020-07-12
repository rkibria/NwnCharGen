cmake_minimum_required(VERSION 3.5)

set( NWN2_FILES_DIR "${CMAKE_CURRENT_BINARY_DIR}/nwn2files" )
file( MAKE_DIRECTORY "${NWN2_FILES_DIR}" )

execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar xv "${NWN2_PATH}/Data/2DA_X2.zip" "2DA_X2/classes.2da"
    WORKING_DIRECTORY "${NWN2_FILES_DIR}"
    )

add_executable( rulesgen rulesgen.cpp )

target_link_libraries( rulesgen PUBLIC
    NWN2DataLib
    Nwn
    )

string( REPLACE "/" "\\\\" NWN2_PATH_WIN "${NWN2_PATH}" )
string( REPLACE "/" "\\\\" NWN2_FILES_DIR_WIN "${NWN2_FILES_DIR}" )

target_compile_definitions( rulesgen
    PRIVATE
    NWN2_PATH="${NWN2_PATH_WIN}"
    EXTRACT_PATH="${NWN2_FILES_DIR_WIN}"
    )
