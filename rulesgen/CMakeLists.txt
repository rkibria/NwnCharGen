cmake_minimum_required(VERSION 3.12)

#############################

find_package( ZLIB REQUIRED )

set( MINIZIP_PATH "" CACHE STRING "Path to zlib's contrib/minizip directory" )
if( NOT IS_DIRECTORY "${MINIZIP_PATH}" )
    message( FATAL_ERROR "must set MINIZIP_PATH" )
endif()

project(minizip_unzip LANGUAGES C)

add_library( minizip_unzip STATIC
    ${MINIZIP_PATH}/iowin32.c
    ${MINIZIP_PATH}/unzip.c
    ${MINIZIP_PATH}/ioapi.c
    )

add_library( minizip::unzip ALIAS minizip_unzip )

target_include_directories( minizip_unzip PUBLIC ${MINIZIP_PATH} )

target_link_libraries( minizip_unzip PUBLIC
    ZLIB::ZLIB
    )

#############################

#execute_process(
#    COMMAND ${CMAKE_COMMAND} -E tar xv "${NWN2_PATH}/Data/2DA.zip" "2DA/cls_savthr_*.*"
#    WORKING_DIRECTORY "${NWN2_FILES_DIR}"
#    )

add_executable( rulesgen
    rulesgen.cpp
    unziphelper.cpp
    )

target_link_libraries( rulesgen PUBLIC
    NWN2DataLib
    Nwn
    minizip::unzip
    Boost::boost
    Boost::filesystem
    )

string( REPLACE "/" "\\\\" NWN2_PATH_WIN "${NWN2_PATH}" )
string( REPLACE "/" "\\\\" OUTPUT_PATH_WIN "${CMAKE_BINARY_DIR}" )

target_compile_definitions( rulesgen
    PRIVATE
    NWN2_PATH="${NWN2_PATH_WIN}"
    OUTPUT_PATH="${OUTPUT_PATH_WIN}"
    )
