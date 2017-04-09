set(DATA_SOURCES "")

macro(ceammc_data_extension mod name)
    list(APPEND DATA_SOURCES "${mod}_${name}.cpp")
endmacro()

ceammc_data_extension(data fifo)
ceammc_data_extension(data list)

ceammc_data_extension(global int)
ceammc_data_extension(global float)
ceammc_data_extension(global list)
ceammc_data_extension(global json)

ceammc_data_extension(local list)
ceammc_data_extension(local int)
ceammc_data_extension(local float)
ceammc_data_extension(local json)

add_library(ceammc_data STATIC mod_data.h mod_data.cpp ${DATA_SOURCES})
if(UNIX)
    set_target_properties(ceammc_data PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
