add_library(ceammc_net STATIC mod_net.cpp net_host.cpp)

target_sources(ceammc_net
        PRIVATE
        net_osc_send.cpp
        net_osc_server.cpp
        net_osc_receive.cpp
        osc_property.cpp)

# libartnet
if(WITH_ARTNET)
    target_compile_definitions(ceammc_net PRIVATE WITH_ARTNET)
    target_sources(ceammc_net
        PRIVATE
        net_artnet_send.cpp
        net_artnet_impl.cpp
    )
    target_include_directories(ceammc_net PUBLIC ${PROJECT_SOURCE_DIR}/ceammc/extra/artnet/libartnet)
    target_link_libraries(ceammc_net PUBLIC artnet)
endif()

find_package(RAGEL 6.6)

if(RAGEL_FOUND)
    RAGEL_TARGET(parser_osc
        parser_osc.rl parser_osc.cpp
        COMPILE_FLAGS "-G2 -s")
    add_custom_target(net_ragel_src SOURCES parser_osc.rl)
endif()

target_sources(ceammc_net PRIVATE parser_osc.cpp)

target_include_directories(ceammc_net
    PUBLIC
        ${PROJECT_SOURCE_DIR}/ceammc/extra/readerwriterqueue # for readerwriterqueue
    PRIVATE
        ${PROJECT_BINARY_DIR} # for config.h
        "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>"
)

# http
target_sources(ceammc_net PRIVATE net_http_send.cpp)
target_link_libraries(ceammc_net PRIVATE http_lib)



