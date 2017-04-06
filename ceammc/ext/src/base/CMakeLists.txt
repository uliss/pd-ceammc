set(BASE_SOURCES msg.cpp)

pd_add_extension(NAME "msg" FILES "msg.cpp" INTERNAL True LIBRARY ceammc LINK ceammc_core)

#if(UNIX)
#    add_custom_command(TARGET msg COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_SONAME_FILE:msg>" "m${PD_EXTERNAL_EXTENSION}")
#elseif(WIN32)
##    add_custom_command(TARGET msg COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_DLLNAME_FILE:msg>" "m${PD_EXTERNAL_EXTENSION}")
#endif()

add_library(ceammc_base STATIC mod_base.cpp ${BASE_SOURCES})
if(UNIX)
    set_target_properties(ceammc_base PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
