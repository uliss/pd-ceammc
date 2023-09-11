set(SPAT_SOURCES
    pan_base.cpp
    pan_linear.cpp
    pan_cosine.cpp
    pan_sqrt.cpp
    pan_spread.cpp
    pan_linsig.cpp
)

set(HOA_SOURCES
    hoa_2d_decoder.cpp
    hoa_2d_encoder.cpp
    hoa_2d_exchanger.cpp
    hoa_2d_map.cpp
    hoa_2d_optim.cpp
    hoa_2d_projector.cpp
    hoa_2d_recomposer.cpp
    hoa_2d_rotate.cpp
    hoa_2d_scope.cpp
    hoa_2d_space.cpp
    hoa_2d_wider.cpp
    hoa_3d_decoder.cpp
    hoa_3d_encoder.cpp
    hoa_3d_exchanger.cpp
    hoa_3d_optim.cpp
    hoa_3d_scope.cpp
    hoa_3d_wider.cpp
    hoa_common.h
    hoa_connections.cpp
    hoa_process.cpp
    hoa_process_inlet.cpp
    hoa_process_instance.cpp
    hoa_process_props.cpp
    ui_2d_map.cpp
)

macro(ceammc_spat_obj_ext name)
    list(APPEND SPAT_SOURCES "spat_${name}.cpp")
    ceammc_faust_gen_obj(spat ${name} JSON)
endmacro()

ceammc_spat_obj_ext(pan4)
ceammc_spat_obj_ext(pan8)
ceammc_spat_obj_ext(zita8)
ceammc_spat_obj_ext(zita6x8)

add_library(ceammc_spat STATIC mod_spat.cpp ${SPAT_SOURCES} ${HOA_SOURCES})

target_compile_definitions(ceammc_spat PRIVATE FAUST_MACRO)
target_compile_definitions(ceammc_spat PUBLIC -DFMT_USE_CONSTEXPR=0)
target_compile_features(ceammc_spat PRIVATE cxx_std_14)

if(APPLE)
    # fix QtCreator parsing error of vDSP.h
    target_compile_options(ceammc_spat PRIVATE -Wno-elaborated-enum-base)
endif()

target_include_directories(ceammc_spat
    PRIVATE
        $<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>
        ${EIGEN3_DIR}
        ${PROJECT_SOURCE_DIR}/ceammc/extra/HoaLibrary-Light/Sources
)

