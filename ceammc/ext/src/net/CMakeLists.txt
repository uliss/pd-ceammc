macro(ceammc_net_external name)
    list(APPEND NET_SOURCES "net_${name}.cpp")
endmacro()

ceammc_net_external(host)

add_library(ceammc_net STATIC mod_net.cpp ${NET_SOURCES})
target_include_directories(ceammc_net
    PRIVATE
        ${PROJECT_SOURCE_DIR}/ceammc/extra # for readerwriterqueue
        ${PROJECT_BINARY_DIR} # for config.h
)

