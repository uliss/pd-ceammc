set(WITH_OPENMP OFF CACHE BOOL " " FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL " " FORCE)
set(WITH_LSR_BINDINGS OFF CACHE BOOL " " FORCE)
set(BUILD_TESTS OFF CACHE BOOL " " FORCE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/code/cmake/Modules")

cmake_policy(SET CMP0075 NEW)
add_subdirectory(code EXCLUDE_FROM_ALL)

if(APPLE)
    if(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL arm64)
        target_compile_definitions(soxr PRIVATE __arm__)
    endif()

    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("-Wno-unused-command-line-argument" WARN_NO_UNUSED_COMMAND_LINE_ARGUMENT)
    if(WARN_NO_UNUSED_COMMAND_LINE_ARGUMENT)
        add_compile_options(-Wno-unused-command-line-argument)
    endif()
endif()
