set(STRING_SOURCES datastring.cpp)
macro(ceammc_string_extension name)
    list(APPEND STRING_SOURCES "string_${name}.cpp")
endmacro()

ceammc_string_extension(str)

add_library(ceammc_string STATIC mod_string.cpp ${STRING_SOURCES})
if(UNIX)
    set_target_properties(ceammc_string PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
