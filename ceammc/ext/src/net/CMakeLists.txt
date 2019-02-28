macro(ceammc_net_external name)
    list(APPEND NET_SOURCES "net_${name}.cpp")
endmacro()

ceammc_net_external(host)

add_library(ceammc_net STATIC mod_net.cpp ${NET_SOURCES})
if(UNIX)
    set_target_properties(ceammc_net PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
