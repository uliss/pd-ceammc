set(SYMBOL_SOURCES "")

macro(ceammc_symbol_external name)
    list(APPEND SYMBOL_SOURCES "symbol_${name}.cpp")
endmacro()

ceammc_symbol_external(length)
ceammc_symbol_external(num_compare)
ceammc_symbol_external(equal)

add_library(ceammc_symbol STATIC mod_symbol.cpp ${SYMBOL_SOURCES})
if(UNIX)
    set_target_properties(ceammc_symbol PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
