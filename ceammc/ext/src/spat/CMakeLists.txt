include_directories(${PROJECT_SOURCE_DIR}/ceammc/extra/HoaLibrary-Light/Sources)
include_directories(${PROJECT_SOURCE_DIR}/ceammc/extra/fmt/include)
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
    hoa_2d_scope.cpp
    hoa_common.h
    hoa_connections.cpp
    hoa_decoder.cpp
    hoa_encoder.cpp
    hoa_optim.cpp
    hoa_process.cpp
    hoa_process_inlet.cpp
    hoa_process_instance.cpp
    hoa_process_props.cpp
    hoa_process_route.cpp
    hoa_projector.cpp
    hoa_recomposer.cpp
    hoa_rotate.cpp
    hoa_thisprocess.cpp
    hoa_wider.cpp
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
