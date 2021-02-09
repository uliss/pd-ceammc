include_directories(../ui/cicm/Sources)

set(TL_SOURCES
    tl_lexer.l
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
ceammc_tl_extension(cue_new)
ceammc_tl_extension(timeline)
ceammc_tl_extension(toggle)
ceammc_tl_extension(transport)


add_library(ceammc_tl STATIC mod_tl.h mod_tl.cpp ${TL_SOURCES})

if(BISON_FOUND)
    add_custom_target(tl_parser_gen
        COMMAND $<TARGET_FILE:reflex-bin> tl_lexer.l
        COMMAND ${BISON_EXECUTABLE} -LC++ tl_parser.y
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

target_include_directories(ceammc_tl PRIVATE $<TARGET_PROPERTY:reflex,INCLUDE_DIRECTORIES>)

add_custom_target(ceammc_tl_tcl_gen
    COMMAND ${PROJECT_SOURCE_DIR}/ceammc/scripts/filecontent_to_cxx_header.sh tl_cue_fn.tcl
    DEPENDS tl_cue_fn.tcl
    SOURCES tl_cue_fn.tcl
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
