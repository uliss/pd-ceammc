set(RANDOM_SOURCES "")
macro(ceammc_random_extension name)
    list(APPEND RANDOM_SOURCES "random_${name}.cpp")
endmacro()

ceammc_random_extension(float)
ceammc_random_extension(int)
ceammc_random_extension(gauss)

add_library(ceammc_random STATIC mod_random.cpp ${RANDOM_SOURCES})
if(UNIX)
    set_target_properties(ceammc_random PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
