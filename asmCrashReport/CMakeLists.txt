cmake_minimum_required(VERSION 3.12)

add_library(asmCrashReport STATIC
    src/asmCrashReport.cpp
    )

target_include_directories(asmCrashReport PUBLIC include)

target_link_libraries(asmCrashReport PRIVATE
    Qt5::Core
    Dbghelp
    )

target_compile_definitions(asmCrashReport PUBLIC ASM_CRASH_REPORT)
