add_library(ceammc_net STATIC mod_net.cpp net_host.cpp)

# liblo
find_package(liblo)
if(LIBLO_FOUND)
    target_sources(ceammc_net
        PRIVATE
        net_osc_send.cpp
        net_osc_server.cpp
        net_osc_receive.cpp
    )
    target_include_directories(ceammc_net PUBLIC ${LIBLO_INCLUDE_DIRS})
    target_link_libraries(ceammc_net PUBLIC ${LIBLO_LIBRARIES})
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

