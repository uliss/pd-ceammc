find_program(CPPCHECK cppcheck)
find_program(CPPCHECK_HTML cppcheck-htmlreport)

set(HTML_OPEN "xdg-open")
if(APPLE)
    set(HTML_OPEN "open")
endif()

if(CPPCHECK)
    message(STATUS "CppCheck found: ${CPPCHECK}. The 'cppcheck' make target is available.")

    set(_CPPCHECK_FLAGS "-DPD_FLOATSIZE=32")
    if(APPLE)
        set(_CPPCHECK_FLAGS "${_CPPCHECK_FLAGS} -U_WINDOWS -U_MSC_VER -U_WIN32 -U__WIN32 -D__MACH__ -D__APPLE__")
    endif()

    add_custom_target(
        ceammc_cppcheck
        COMMAND ${CPPCHECK}
        ${CMAKE_CURRENT_SOURCE_DIR}
        --enable=warning,performance,portability,information,missingInclude
        --suppressions-list=${PROJECT_SOURCE_DIR}/ceammc/cppcheck_suppressions.txt
        -i src/proto/firmata/firmata_bison.tab.c
        -i data/rapidjson
        -i data/rapidjson/document.h
        -i lib/dict_parser.tab.c
        -i lib/lex.dict_parser.c
        -i lib/lex.mlist_parser.c
        -i lib/mlist_parser.tab.c
        -i lib/utf8rewind
        -i math/lex.math_expr.c
        -i math/math_expr.tab.c
        -i lib/dict_parser_impl.cpp
        -i lib/mlist_parser_impl.cpp
        -i exp/
        -i rtmidi/
        ${_CPPCHECK_FLAGS}
        --std=c++11
        --xml
        --xml-version=2
        --output-file=${CMAKE_BINARY_DIR}/cppcheck_result.xml
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

    add_custom_target(
        cppcheck
        DEPENDS ceammc_cppcheck
        COMMAND ${CPPCHECK_HTML}
        --file ${CMAKE_BINARY_DIR}/cppcheck_result.xml
        --source-dir "${CMAKE_CURRENT_SOURCE_DIR}"
        --report-dir ${CMAKE_BINARY_DIR}/cppcheck_html
        COMMAND ${HTML_OPEN} ${CMAKE_BINARY_DIR}/cppcheck_html/index.html
    )
endif()
