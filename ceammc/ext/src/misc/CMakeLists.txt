set(MISC_SOURCES "")
macro(ceammc_misc_extension mod name)
    list(APPEND MISC_SOURCES "${mod}_${name}.cpp")
    ceammc_faust_gen(${mod} ${name})
endmacro()

ceammc_misc_extension(live capture)

find_package(ModPlug)
if(MODPLUG_FOUND)
    include_directories(${MODPLUG_FOUND_INCLUDES})
    ceammc_misc_extension(misc modplug)
endif()

add_library(ceammc_misc STATIC mod_misc.cpp ${MISC_SOURCES})
set_target_properties(ceammc_misc PROPERTIES COMPILE_DEFINITIONS "FAUST_MACRO")
if(UNIX)
    set_target_properties(ceammc_misc PROPERTIES COMPILE_FLAGS "-fPIC")
endif()

if(MODPLUG_FOUND)
    target_link_libraries(ceammc_misc ${MODPLUG_LIBRARIES})
endif()
