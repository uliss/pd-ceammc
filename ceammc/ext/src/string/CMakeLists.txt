macro(ceammc_string_extension name)
    list(APPEND STRING_SOURCES "string_${name}.cpp")
endmacro()

ceammc_string_extension(contains)
ceammc_string_extension(ends_with)
ceammc_string_extension(equal)
ceammc_string_extension(format)
ceammc_string_extension(join)
ceammc_string_extension(length)
ceammc_string_extension(remove)
ceammc_string_extension(replace)
ceammc_string_extension(split)
ceammc_string_extension(starts_with)
ceammc_string_extension(str)
ceammc_string_extension(to_symbol)
ceammc_string_extension(substr)
ceammc_string_extension(match)

list(APPEND STRING_SOURCES
    string_property.cpp
    symbol_equal.cpp
    symbol_length.cpp
    symbol_num_compare.cpp
    symbol_to_any.cpp
    symbol_to_int.cpp
)

add_library(ceammc_string STATIC mod_string.cpp ${STRING_SOURCES})

target_include_directories(ceammc_string
    PRIVATE "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>"
            "$<TARGET_PROPERTY:re2,INCLUDE_DIRECTORIES>")
