add_subdirectory(firmata)

add_library(ceammc_proto STATIC mod_proto.cpp mpv_ipc.cpp)

function(ceammc_proto_external name)
    target_sources(ceammc_proto PRIVATE "proto_${name}.cpp")
endfunction()

ceammc_proto_external(firmata)
ceammc_proto_external(feelworld)
ceammc_proto_external(hui)
ceammc_proto_external(inscore)
ceammc_proto_external(midi)
ceammc_proto_external(midi_casio)
ceammc_proto_external(midi_cc)
ceammc_proto_external(midi_sysex)
ceammc_proto_external(moppy)
ceammc_proto_external(morse)
ceammc_proto_external(mpv)
ceammc_proto_external(sp_alpaca)
ceammc_proto_external(vlc)
ceammc_proto_external(whammy)
ceammc_proto_external(xtouch_ext)

find_package(RAGEL 6.6)
if(RAGEL_FOUND)
    RAGEL_TARGET(parser_whammy
        parser_whammy.rl parser_whammy.cpp
        COMPILE_FLAGS "-G2 -s")
    RAGEL_TARGET(parser_vlc
        parser_vlc.rl parser_vlc.cpp
        COMPILE_FLAGS "-G2 -s")

    add_custom_target(parser_src SOURCES parser_whammy.rl parser_vlc.rl)
endif()

if(WITH_LTC)
    find_package(LTC)
    if(LTC_FOUND)
        target_include_directories(ceammc_proto PRIVATE ${LTC_INCLUDE_DIRS})
        target_compile_definitions(ceammc_proto PRIVATE -DWITH_LIBLTC)
        target_link_libraries(ceammc_proto PUBLIC ${LTC_LIBRARIES})
    endif()
    target_sources(ceammc_proto PRIVATE proto_ltc_in_tilde.cpp)
    target_sources(ceammc_proto PRIVATE proto_ltcout_tilde.cpp)
endif()

target_sources(ceammc_proto
    PRIVATE
        parser_vlc.cpp
        parser_whammy.cpp
        proto_midi_parser.cpp
)
target_include_directories(ceammc_proto
    PUBLIC
        ${PROJECT_SOURCE_DIR}/ceammc/extra/readerwriterqueue
    PRIVATE
        ${PROJECT_BINARY_DIR} # for config.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../midi
        "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>"
)
target_link_libraries(ceammc_proto PRIVATE proto_firmata http_lib)
