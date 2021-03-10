include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-fno-fast-math GLITCH_NO_FAST_MATH)

add_library(glitch-core STATIC
    glitchxx.cpp
    glitch/core/glitch.c
)

if(${GLITCH_NO_FAST_MATH})
    target_compile_options(glitch-core PRIVATE -fno-fast-math)
endif()

target_include_directories(glitch-core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/glitch/core)
