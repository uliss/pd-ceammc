set(SYMBOL_SOURCES "")
macro(ceammc_symbol_extension name)
    list(APPEND SYMBOL_SOURCES "symbol_${name}.cpp")
endmacro()

ceammc_symbol_extension(ends_with)

if(GLIB_FOUND)
    include_directories(${GLIB_INCLUDE_DIRS})
    add_definitions(-DWITH_GLIB)

    ceammc_symbol_extension(compare)
    ceammc_symbol_extension(length)

#    ceammc_glib_extension(string join)
#    ceammc_glib_extension(string slice)
#    ceammc_glib_extension(string split)
endif()

add_library(ceammc_symbol STATIC mod_symbol.cpp ${SYMBOL_SOURCES})
if(UNIX)
    set_target_properties(ceammc_symbol PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
