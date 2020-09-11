set(RANDOM_SOURCES mod_random.cpp rnd_gen.cpp)

macro(ceammc_random_extension name)
    list(APPEND RANDOM_SOURCES "random_${name}.cpp")
endmacro()

ceammc_random_extension(atom)
ceammc_random_extension(discrete)
ceammc_random_extension(float)
ceammc_random_extension(gauss)
ceammc_random_extension(int)
ceammc_random_extension(linear)
ceammc_random_extension(pwconst)
ceammc_random_extension(pwlin)

add_library(ceammc_random STATIC ${RANDOM_SOURCES})
target_include_directories(ceammc_random
    PRIVATE "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>")
