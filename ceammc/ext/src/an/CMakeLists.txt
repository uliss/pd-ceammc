set(ANALYZE_SOURCES mod_analyze.h mod_analyze.cpp)

macro(ceammc_an_external name)
    list(APPEND ANALYZE_SOURCES "an_${name}.cpp")
endmacro()

ceammc_an_external(aubio_onset)
ceammc_an_external(aubio_onset_tilde)
ceammc_an_external(pitchtrack)

add_library(ceammc_analyze STATIC ${ANALYZE_SOURCES})
target_include_directories(ceammc_analyze
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${PROJECT_SOURCE_DIR}/src
    PUBLIC ${PROJECT_SOURCE_DIR}/ceammc/extra
    PUBLIC ${PROJECT_SOURCE_DIR}/ceammc/extra/aubio/aubio/src
)

target_link_libraries(ceammc_analyze PUBLIC helmholtz_ptrack aubio)

if(UNIX)
    target_compile_options(ceammc_analyze PRIVATE "-fPIC")
endif()
