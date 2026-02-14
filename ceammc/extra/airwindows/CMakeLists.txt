add_library(airw_infinity
    STATIC
    Infinity/Infinity.cpp
    Infinity/InfinityProc.cpp
)

target_include_directories(airw_infinity
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inifinity
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# reduce number of warnings in third-party code
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-Wno-multichar" WARN_NO_MULTICHAR)
check_cxx_compiler_flag("-Wno-unused-value" WARN_NO_UNUSED_VALUE)

if(WARN_NO_MULTICHAR)
    target_compile_options(airw_infinity PRIVATE -Wno-multichar)
endif()

if(WARN_NO_UNUSED_VALUE)
    target_compile_options(airw_infinity PRIVATE -Wno-unused-value)
endif()


