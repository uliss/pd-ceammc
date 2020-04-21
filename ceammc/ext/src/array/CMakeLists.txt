set(ARRAY_SOURCES array_base.cpp)

macro(ceammc_array_extension name)
    list(APPEND ARRAY_SOURCES "array_${name}.cpp")
endmacro()

ceammc_array_extension(bpm)
ceammc_array_extension(copy)
ceammc_array_extension(do)
ceammc_array_extension(each)
ceammc_array_extension(fill)
ceammc_array_extension(hist)
ceammc_array_extension(mean)
ceammc_array_extension(minmax)
ceammc_array_extension(p2s)
ceammc_array_extension(resample)
ceammc_array_extension(rms)
ceammc_array_extension(s2p)
ceammc_array_extension(set)
ceammc_array_extension(stddev)
ceammc_array_extension(stretch)
ceammc_array_extension(sum)
ceammc_array_extension(sum2)
ceammc_array_extension(variance)
ceammc_array_extension(vline_play)
ceammc_array_extension(window)

add_library(ceammc_array STATIC mod_array.h mod_array.cpp ${ARRAY_SOURCES})

target_include_directories(ceammc_array PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
    "$<TARGET_PROPERTY:soundtouch,INCLUDE_DIRECTORIES>"
    "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>"
    "$<TARGET_PROPERTY:soxr,INCLUDE_DIRECTORIES>"
)

target_link_libraries(ceammc_array PRIVATE soundtouch)
target_link_libraries(ceammc_array PRIVATE soxr)

if(UNIX)
    set_target_properties(ceammc_array PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
