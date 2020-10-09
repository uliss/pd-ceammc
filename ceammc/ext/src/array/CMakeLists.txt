set(ARRAY_SOURCES
    array_base.cpp
    grain.cpp
    grain.h
    grain_random.cpp
    grain_random.h
    plot_linspace_tilde.cpp
    plot_logspace_tilde.cpp
    plot_geomspace_tilde.cpp
    plot_hist_tilde.cpp
    plot_response_tilde.cpp
)

macro(ceammc_array_external name)
    list(APPEND ARRAY_SOURCES "array_${name}.cpp")
endmacro()

ceammc_array_external(bpm)
ceammc_array_external(copy)
ceammc_array_external(do)
ceammc_array_external(each)
ceammc_array_external(fill)
ceammc_array_external(grainer)
ceammc_array_external(hist)
ceammc_array_external(mean)
ceammc_array_external(minmax)
ceammc_array_external(p2s)
ceammc_array_external(play_tilde)
ceammc_array_external(plot_tilde)
ceammc_array_external(resample)
ceammc_array_external(rms)
ceammc_array_external(s2p)
ceammc_array_external(set)
ceammc_array_external(stddev)
ceammc_array_external(stretch)
ceammc_array_external(sum)
ceammc_array_external(sum2)
ceammc_array_external(variance)
ceammc_array_external(vline_play)
ceammc_array_external(window)

add_library(ceammc_array STATIC mod_array.h mod_array.cpp ${ARRAY_SOURCES})
target_include_directories(ceammc_array PRIVATE "${PROJECT_SOURCE_DIR}/ceammc/extra/muparser/muparser/include")

if(UNIX)
    target_link_libraries(ceammc_array PRIVATE soxr)
endif()

if(BISON_FOUND)
    add_custom_target(ceammc_array_gen
        COMMAND ${BISON_EXECUTABLE} grain_properties.yy
        SOURCES grain_properties.yy
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

target_sources(ceammc_array PRIVATE
    grainprops.parser.hpp grainprops.parser.cpp
    grainprops.lexer.h grainprops.lexer.cpp
)

target_include_directories(ceammc_array PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
    "$<TARGET_PROPERTY:soundtouch,INCLUDE_DIRECTORIES>"
    "$<TARGET_PROPERTY:soxr,INCLUDE_DIRECTORIES>"
    "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>"
)

target_link_libraries(ceammc_array PRIVATE
    soundtouch
    reflex
)
