set(SYMBOL_SOURCES "")

macro(ceammc_symbol_extension name)
    list(APPEND SYMBOL_SOURCES "symbol_${name}.cpp")
endmacro()

ceammc_symbol_extension(ends_with)
ceammc_symbol_extension(length)
ceammc_symbol_extension(num_compare)
ceammc_symbol_extension(equal)

#    ceammc_symbol_extension(string join)
#    ceammc_symbol_extension(string slice)
#    ceammc_symbol_extension(string split)

add_library(ceammc_symbol STATIC mod_symbol.cpp ${SYMBOL_SOURCES})
if(UNIX)
    set_target_properties(ceammc_symbol PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
