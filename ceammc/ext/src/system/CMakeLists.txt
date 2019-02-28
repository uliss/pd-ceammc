include_directories(${CMAKE_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR}/ceammc/extra/subprocess/tiny-process-library)

set(SYSTEM_SOURCES "")

macro(ceammc_system_extension name)
    list(APPEND SYSTEM_SOURCES "system_${name}.cpp")
endmacro()

ceammc_system_extension(memsize)
ceammc_system_extension(memused)
ceammc_system_extension(screen_size)
ceammc_system_extension(getenv)
ceammc_system_extension(hostname)
ceammc_system_extension(shell)


add_library(ceammc_system STATIC mod_system.cpp ${SYSTEM_SOURCES})
if(UNIX)
    set_target_properties(ceammc_system PROPERTIES COMPILE_FLAGS "-fPIC")
endif()

target_link_libraries(ceammc_system tiny-process-library)

