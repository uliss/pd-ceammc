set(WINDOW_SOURCES window.cpp)

add_library(ceammc_window STATIC mod_window.cpp ${WINDOW_SOURCES})
if(UNIX)
    set_target_properties(ceammc_window PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
