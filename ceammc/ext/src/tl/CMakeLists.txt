include_directories(../ui/cicm/Sources)

set(TL_SOURCES
    tl_lexer.lex
    tl_parser.y
    tl_lexer.cpp
    tl_parser.cpp
    tl_cmd_driver.cpp
    tl_eventlist.cpp
    )

macro(ceammc_tl_extension name)
     list(APPEND TL_SOURCES "tl_${name}.cpp")
endmacro()

ceammc_tl_extension(bang)
ceammc_tl_extension(baseobject)
ceammc_tl_extension(cue)
ceammc_tl_extension(cue_new)
ceammc_tl_extension(timeline)
ceammc_tl_extension(toggle)
ceammc_tl_extension(transport)


add_library(ceammc_tl STATIC mod_tl.h mod_tl.cpp ${TL_SOURCES})
if(UNIX)
    set_target_properties(ceammc_tl PROPERTIES COMPILE_FLAGS "-fPIC")
endif()

if(BISON_FOUND AND FLEX_FOUND)
    add_custom_target(tl_parser_gen
        COMMAND ${FLEX_EXECUTABLE} tl_lexer.lex
        COMMAND ${BISON_EXECUTABLE} -LC++ tl_parser.y
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()
