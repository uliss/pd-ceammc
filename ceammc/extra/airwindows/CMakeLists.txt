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
