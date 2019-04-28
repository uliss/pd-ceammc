include_directories(${PROJECT_SOURCE_DIR}/ceammc/extra/HoaLibrary-Light/Sources)
include_directories(${EIGEN3_DIR})

set(SPAT_SOURCES
    pan_base.cpp
    pan_linear.cpp
    pan_cosine.cpp
    pan_sqrt.cpp
    pan_spread.cpp
    pan_linsig.cpp
)

set(HOA_SOURCES
    hoa_common.h
    hoa_decoder.cpp
    hoa_encoder.cpp
    hoa_optim.cpp
    hoa_rotate.cpp
)

macro(ceammc_spat_obj_ext name)
    list(APPEND SPAT_SOURCES "spat_${name}.cpp")
    ceammc_faust_gen_obj(spat ${name} JSON)
endmacro()

ceammc_spat_obj_ext(pan4)
ceammc_spat_obj_ext(pan8)

add_library(ceammc_spat STATIC mod_spat.cpp ${SPAT_SOURCES} ${HOA_SOURCES})
set_target_properties(ceammc_spat PROPERTIES COMPILE_DEFINITIONS "FAUST_MACRO")
set_property(TARGET ceammc_spat PROPERTY CXX_STANDARD 14)
if(UNIX)
    set_target_properties(ceammc_spat PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
