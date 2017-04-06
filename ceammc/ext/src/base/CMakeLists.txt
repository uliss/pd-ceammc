set(BASE_SOURCES msg.cpp prop.get prop.set)

pd_add_extension(NAME "msg" FILES "msg.cpp" INTERNAL True LIBRARY ceammc LINK ceammc_core)

add_library(ceammc_base STATIC mod_base.cpp ${BASE_SOURCES})
if(UNIX)
    set_target_properties(ceammc_base PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
