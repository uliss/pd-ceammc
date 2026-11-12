add_library(ceammc_net STATIC mod_net.cpp net_host.cpp osc_property.cpp)

function(ceammc_net_external name)
  target_sources(ceammc_net PRIVATE "net_${name}.h" "net_${name}.cpp")
endfunction()

ceammc_net_external(osc_send)
ceammc_net_external(osc_server)
ceammc_net_external(osc_receive)

# MQTT
target_sources(ceammc_net PRIVATE net_mqtt_client.cpp)
if(WITH_MQTT)
    target_compile_definitions(ceammc_net PRIVATE WITH_MQTT)
    if(UNIX AND NOT APPLE)
        find_package(OpenSSL REQUIRED)
        target_link_libraries(ceammc_net PRIVATE ${OPENSSL_LIBRARIES})
    endif()
endif()

# telegram bot
target_sources(ceammc_net PRIVATE net_telegram_bot.cpp)
if(WITH_TELEGRAM)
    target_include_directories(ceammc_net PRIVATE
        PRIVATE
            ${PROJECT_SOURCE_DIR}/ceammc/extra/rust/net
    )
    target_link_libraries(ceammc_net PRIVATE net_rust)
    target_compile_definitions(ceammc_net PRIVATE WITH_TELEGRAM)
    if(UNIX AND NOT APPLE)
        find_package(OpenSSL REQUIRED)
        target_link_libraries(ceammc_net PRIVATE ${OPENSSL_LIBRARIES})
    endif()
endif()

# libartnet
ceammc_net_external(artnet_send)
if(WITH_ARTNET)
    ceammc_net_external(artnet_impl)
    target_compile_definitions(ceammc_net PRIVATE WITH_ARTNET)
    target_include_directories(ceammc_net PRIVATE ${PROJECT_SOURCE_DIR}/ceammc/extra/artnet/libartnet)
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
target_sources(ceammc_net PRIVATE net_http_send.cpp net_http_client.cpp)
if(WITH_HTTP)
    target_compile_definitions(ceammc_net PRIVATE WITH_HTTP)
    target_sources(ceammc_net PRIVATE net_http_send.cpp net_http_client.cpp)
    target_link_libraries(ceammc_net PRIVATE http_lib net_rust)
    target_include_directories(ceammc_net
        PRIVATE
            ${PROJECT_SOURCE_DIR}/ceammc/extra/rust/net
        )
    if(UNIX AND NOT APPLE)
        find_package(OpenSSL REQUIRED)
        target_link_libraries(ceammc_net PRIVATE ${OPENSSL_LIBRARIES})
    endif()
endif()

# freesound
target_sources(ceammc_net PRIVATE net_freesound.cpp)
if(WITH_FREESOUND)
    target_compile_definitions(ceammc_net PRIVATE WITH_FREESOUND)
    target_sources(ceammc_net PRIVATE net_freesound.cpp)
    target_link_libraries(ceammc_net PRIVATE net_rust)
    target_include_directories(ceammc_net
        PRIVATE
            ${PROJECT_SOURCE_DIR}/ceammc/extra/rust/net
        )
    if(UNIX AND NOT APPLE)
        find_package(OpenSSL REQUIRED)
        target_link_libraries(ceammc_net PRIVATE ${OPENSSL_LIBRARIES})
    endif()
endif()

# MDNS
target_sources(ceammc_net PRIVATE net_mdns.cpp)
if(WITH_ZEROCONF)
    target_compile_definitions(ceammc_net PRIVATE WITH_ZEROCONF)
    target_link_libraries(ceammc_net PRIVATE core_rust)
    target_include_directories(ceammc_net
        PRIVATE
            ${PROJECT_SOURCE_DIR}/ceammc/extra/rust/core
    )
endif()

# websocket
ceammc_net_external(ws_client)
ceammc_net_external(ws_server)
if(WITH_WEBSOCKET)
    target_compile_definitions(ceammc_net PRIVATE WITH_WEBSOCKET)
    if(UNIX AND NOT APPLE)
        find_package(OpenSSL REQUIRED)
        target_link_libraries(ceammc_net PRIVATE ${OPENSSL_LIBRARIES})
    endif()
endif()

# esphome
ceammc_net_external(esphome_client)
if(WITH_ESPHOME)
    target_compile_definitions(ceammc_net PRIVATE WITH_ESPHOME)
    pd_method_args(ceammc_net net_esphome_client)
endif()

# wled
ceammc_net_external(wled)
if(WITH_WLED)
    target_compile_definitions(ceammc_net PRIVATE WITH_WLED)
    pd_method_args(ceammc_net net_wled)
endif()

if(WITH_RUST_NET)
    target_include_directories(ceammc_net PRIVATE
        PRIVATE
            ${PROJECT_SOURCE_DIR}/ceammc/extra/rust/net
            ${PROJECT_SOURCE_DIR}/ceammc/extra/rust/msg
    )
    target_link_libraries(ceammc_net PUBLIC net_rust)
endif()



