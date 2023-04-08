add_library(ceammc_music STATIC mod_music.cpp)
target_include_directories(ceammc_music
    PRIVATE
        "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>")

target_sources(ceammc_music PRIVATE
    duration_to_time.cpp
    rhythm_euclid.cpp
    rhythm_hexbeat.cpp
    rhythm_lsystem.cpp
    voice_to_midi.cpp
)
