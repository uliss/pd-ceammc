add_library(glitch-core STATIC
    glitchxx.cpp
    glitch/core/glitch.c
)

target_include_directories(glitch-core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/glitch/core)
