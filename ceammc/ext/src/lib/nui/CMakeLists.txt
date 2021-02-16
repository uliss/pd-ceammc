add_library(ceammc_nui STATIC
    nui.cpp
)

target_include_directories(ceammc_nui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
