add_library(ceammc_data STATIC
    mod_data.h mod_data.cpp
    dict_base.cpp
    local_base.cpp
    editor_data.cpp
)

function(ceammc_data_external mod name)
    target_sources(ceammc_data PRIVATE "${mod}_${name}.cpp")
endfunction()

ceammc_data_external(color mix)
ceammc_data_external(color scale)

ceammc_data_external(data bimap)
ceammc_data_external(data color)
ceammc_data_external(data copy)
ceammc_data_external(data dict)
ceammc_data_external(data fifo)
ceammc_data_external(data float)
ceammc_data_external(data int)
ceammc_data_external(data list)
ceammc_data_external(data mlist)
ceammc_data_external(data set)

ceammc_data_external(dict contains)
ceammc_data_external(dict each)
ceammc_data_external(dict from_list)
ceammc_data_external(dict get)
ceammc_data_external(dict is)
ceammc_data_external(dict keys)
ceammc_data_external(dict pass)
ceammc_data_external(dict reject)
ceammc_data_external(dict size)
ceammc_data_external(dict to_list)
ceammc_data_external(dict values)

ceammc_data_external(mlist flatten)

ceammc_data_external(set contains)
ceammc_data_external(set difference)
ceammc_data_external(set equal)
ceammc_data_external(set intersection)
ceammc_data_external(set size)
ceammc_data_external(set symdiff)
ceammc_data_external(set to_list)
ceammc_data_external(set union)

ceammc_data_external(global dict)
ceammc_data_external(global float)
ceammc_data_external(global int)
ceammc_data_external(global list)
ceammc_data_external(global mlist)
ceammc_data_external(global set)

ceammc_data_external(local dict)
ceammc_data_external(local float)
ceammc_data_external(local int)
ceammc_data_external(local list)
ceammc_data_external(local mlist)
ceammc_data_external(local set)

ceammc_data_external(rtree to_list)

target_include_directories(ceammc_data PRIVATE "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>")
target_link_libraries(ceammc_data PRIVATE colorm)

