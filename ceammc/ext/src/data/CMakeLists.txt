set(DATA_SOURCES
    datatype_dict.cpp
    datatype_set.cpp
)

macro(ceammc_data_extension mod name)
    list(APPEND DATA_SOURCES "${mod}_${name}.cpp")
endmacro()

ceammc_data_extension(data dict)
ceammc_data_extension(data fifo)
ceammc_data_extension(data float)
ceammc_data_extension(data int)
ceammc_data_extension(data list)
ceammc_data_extension(data mlist)
ceammc_data_extension(data set)

ceammc_data_extension(dict contains)
ceammc_data_extension(dict get)
ceammc_data_extension(dict keys)
ceammc_data_extension(dict size)
ceammc_data_extension(dict values)

ceammc_data_extension(mlist flatten)

ceammc_data_extension(set contains)
ceammc_data_extension(set difference)
ceammc_data_extension(set equal)
ceammc_data_extension(set intersection)
ceammc_data_extension(set size)
ceammc_data_extension(set symdiff)
ceammc_data_extension(set to_list)
ceammc_data_extension(set union)

ceammc_data_extension(global dict)
ceammc_data_extension(global float)
ceammc_data_extension(global int)
ceammc_data_extension(global list)
ceammc_data_extension(global mlist)
ceammc_data_extension(global set)

ceammc_data_extension(local dict)
ceammc_data_extension(local float)
ceammc_data_extension(local int)
ceammc_data_extension(local list)
ceammc_data_extension(local mlist)
ceammc_data_extension(local set)

set(MLIST_SOURCES
    datatype_mlist.cpp
    mlist_parser.lex
    mlist_parser.y
    mlist_parser.tab.c
    lex.mlist_parser.c
    mlist_parser_impl.cpp)

set(DICT_PARSER_SOURCES
    dict_parser.lex
    dict_parser.y
    dict_parser.tab.c
    lex.dict_parser.c
    dict_parser_impl.cpp
    )

if(BISON_FOUND AND FLEX_FOUND)
    add_custom_target(data_mlist_parser_gen
        COMMAND ${FLEX_EXECUTABLE} --header-file=lex.mlist_parser.h mlist_parser.lex
        COMMAND ${BISON_EXECUTABLE} -d mlist_parser.y
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

    add_custom_target(data_dict_parser_gen
        COMMAND ${FLEX_EXECUTABLE} --header-file=lex.dict_parser.h dict_parser.lex
        COMMAND ${BISON_EXECUTABLE} -d dict_parser.y
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

list(APPEND DATA_SOURCES ${MLIST_SOURCES} ${DICT_PARSER_SOURCES})

add_library(ceammc_data STATIC mod_data.h mod_data.cpp ${DATA_SOURCES})
if(UNIX)
    set_target_properties(ceammc_data PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
