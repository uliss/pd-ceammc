add_definitions(-DPD)
include_directories(${PROJECT_SOURCE_DIR}/src)

add_library(helmholtz_ptrack STATIC Helmholtz.h Helmholtz.cpp)

if(UNIX)
    set_target_properties(helmholtz_ptrack PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
