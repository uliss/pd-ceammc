set(SPAT_SOURCES
    pan_base.cpp
    pan_linear.cpp
    pan_cosine.cpp
    pan_sqrt.cpp
    pan_spread.cpp
    pan_linsig.cpp
)

macro(ceammc_spat_extension name)
    list(APPEND SPAT_SOURCES "spat_${name}.cpp")
    ceammc_faust_gen(spat ${name})
endmacro()

ceammc_spat_extension(pan4)
ceammc_spat_extension(pan8)

add_library(ceammc_spat STATIC mod_spat.cpp ${SPAT_SOURCES})
set_target_properties(ceammc_spat PROPERTIES COMPILE_DEFINITIONS "FAUST_MACRO")
if(UNIX)
    set_target_properties(ceammc_spat PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
