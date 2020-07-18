cmake_minimum_required( VERSION 3.12 )

#execute_process(
#    COMMAND ${CMAKE_COMMAND} -E tar xv "${NWN2_PATH}/Data/2DA.zip" "2DA/cls_savthr_*.*"
#    WORKING_DIRECTORY "${NWN2_FILES_DIR}"
#    )

add_executable( rulesgen
    rulesgen.cpp
    twodamapper.cpp
    )

target_link_libraries( rulesgen PUBLIC
    NWN2DataLib
    Nwn
    UnzipHelper::UnzipHelper
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
