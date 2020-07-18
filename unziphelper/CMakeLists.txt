cmake_minimum_required( VERSION 3.12 )

find_package( ZLIB REQUIRED )

set( MINIZIP_PATH "" CACHE STRING "Path to zlib's contrib/minizip directory" )
if( NOT IS_DIRECTORY "${MINIZIP_PATH}" )
    message( FATAL_ERROR "must set MINIZIP_PATH" )
endif()

project( minizip_unzip LANGUAGES C )

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


project( UnzipHelper LANGUAGES CXX )

add_library( UnzipHelper STATIC
    src/unziphelper.cpp
    )

add_library( UnzipHelper::UnzipHelper ALIAS UnzipHelper )

target_link_libraries( UnzipHelper PUBLIC
    minizip::unzip
    )

target_include_directories( UnzipHelper PUBLIC include )
