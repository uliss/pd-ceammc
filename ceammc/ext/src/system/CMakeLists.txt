include_directories(${CMAKE_BINARY_DIR})

set(SYSTEM_SOURCES "")

macro(ceammc_system_extension name)
    list(APPEND SYSTEM_SOURCES "system_${name}.cpp")
endmacro()

ceammc_system_extension(memsize)
ceammc_system_extension(memused)
ceammc_system_extension(screen_size)
ceammc_system_extension(getenv)
ceammc_system_extension(hostname)


add_library(ceammc_system STATIC mod_system.cpp ${SYSTEM_SOURCES})
if(UNIX)
    set_target_properties(ceammc_system PROPERTIES COMPILE_FLAGS "-fPIC")
endif()

