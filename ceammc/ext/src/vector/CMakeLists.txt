set(VECTOR_SOURCES "")
macro(ceammc_vector_extension name)
    list(APPEND VECTOR_SOURCES "vector_${name}.cpp")
endmacro()

ceammc_vector_extension(length)
ceammc_vector_extension(normalize)
ceammc_vector_extension(distance)
ceammc_vector_extension(dot)

add_library(ceammc_vector STATIC mod_vector.cpp ${VECTOR_SOURCES})
if(UNIX)
    set_target_properties(ceammc_vector PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
