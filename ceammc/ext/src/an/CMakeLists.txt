include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/ceammc/extra)

set(ANALYZE_SOURCES mod_analyze.h mod_analyze.cpp)

macro(ceammc_an_external name)
    list(APPEND ANALYZE_SOURCES "an_${name}.cpp")
endmacro()

ceammc_an_external(pitchtrack)

add_library(ceammc_analyze STATIC ${ANALYZE_SOURCES})
target_link_libraries(ceammc_analyze helmholtz_ptrack)
if(UNIX)
    set_target_properties(ceammc_analyze PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
