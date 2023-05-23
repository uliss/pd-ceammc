add_library(ceammc_pieces STATIC mod_pieces.cpp)
target_compile_definitions(ceammc_pieces PRIVATE FAUST_MACRO)
target_include_directories(ceammc_pieces PRIVATE "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>")

macro(ceammc_add_piece name)
    target_sources(ceammc_pieces PRIVATE "piece_${name}.cpp")
    ceammc_faust_gen_obj(piece ${name})
endmacro()

ceammc_add_piece(stok_solo)
ceammc_add_piece(nono_quando_fb1)


