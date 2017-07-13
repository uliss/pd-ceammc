set(MISC_SOURCES "")
macro(ceammc_misc_extension mod name)
    list(APPEND MISC_SOURCES "${mod}_${name}.cpp")
    ceammc_faust_gen(${mod} ${name})
endmacro()

ceammc_misc_extension(live capture)

find_package(MikMod)
if(MIKMOD_FOUND)
    add_definitions(-DMIKMOD_FOUND)
    include_directories(${MIKMOD_INCLUDES})
    ceammc_misc_extension(misc mikmod)
endif()

add_library(ceammc_misc STATIC mod_misc.cpp ${MISC_SOURCES})
set_target_properties(ceammc_misc PROPERTIES COMPILE_DEFINITIONS "FAUST_MACRO")
if(UNIX)
    set_target_properties(ceammc_misc PROPERTIES COMPILE_FLAGS "-fPIC")
endif()

if(MIKMOD_FOUND)
    target_link_libraries(ceammc_misc ${MIKMOD_LIBRARIES})
endif()
