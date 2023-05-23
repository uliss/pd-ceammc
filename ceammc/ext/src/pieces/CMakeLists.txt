add_library(ceammc_pieces STATIC mod_pieces.cpp)
target_compile_definitions(ceammc_pieces PRIVATE FAUST_MACRO)
target_include_directories(ceammc_pieces PRIVATE "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>")

macro(ceammc_add_piece name)
    target_sources(ceammc_pieces PRIVATE "piece_${name}.cpp")
    ceammc_faust_gen_obj(piece ${name})
endmacro()

macro(ceammc_add_piece_double name)
    target_sources(ceammc_pieces PRIVATE "piece_${name}.cpp")
    ceammc_faust_gen_obj(piece ${name} DOUBLE)
endmacro()

ceammc_add_piece(stok_solo)
ceammc_add_piece_double(nono_quando_fb1)
ceammc_add_piece_double(nono_quando_fb2)
ceammc_add_piece_double(nono_quando_h1)
ceammc_add_piece_double(nono_quando_h2)
ceammc_add_piece_double(nono_quando_h3)
ceammc_add_piece_double(nono_quando_vocoder)


