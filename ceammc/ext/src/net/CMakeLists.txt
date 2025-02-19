add_library(ceammc_net STATIC mod_net.cpp net_host.cpp)

target_sources(ceammc_net
    PRIVATE
        net_osc_send.cpp
        net_osc_server.cpp
        net_osc_receive.cpp
        osc_property.cpp)

# MQTT
target_sources(ceammc_net PRIVATE net_mqtt_client.cpp)
if(WITH_MQTT)
    target_include_directories(ceammc_net PRIVATE
        PRIVATE
            ${PROJECT_SOURCE_DIR}/ceammc/extra/rust/net
    )
    target_link_libraries(ceammc_net PRIVATE net_rust)
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
target_sources(ceammc_net PRIVATE net_ws_client.cpp net_ws_server.cpp)
if(WITH_WEBSOCKET)
    target_compile_definitions(ceammc_net PRIVATE WITH_WEBSOCKET)
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



