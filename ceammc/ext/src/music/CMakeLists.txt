add_library(ceammc_music STATIC mod_music.cpp)

target_sources(ceammc_music PRIVATE
    bpm_property.cpp
    duration_to_time.cpp
)
