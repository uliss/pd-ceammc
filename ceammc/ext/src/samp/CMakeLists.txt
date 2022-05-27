add_library(ceammc_samp STATIC mod_samp.cpp)

target_sources(ceammc_samp PRIVATE
    samp_time_tilde.cpp
)
