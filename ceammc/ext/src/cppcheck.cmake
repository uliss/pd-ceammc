find_program(CPPCHECK cppcheck)
find_program(CPPCHECK_HTML cppcheck-htmlreport)

set(HTML_OPEN "xdg-open")
if(APPLE)
    set(HTML_OPEN "open")
endif()

file(GLOB_RECURSE CEAMMC_ALL_SOURCE_FILES "*.cpp" "*.h")

if(CPPCHECK)
    message(STATUS "CppCheck found: ${CPPCHECK}. The 'ceammc_cppcheck' make target is available.")

    add_custom_command(
        OUTPUT cppcheck_result.xml
        DEPENDS ${CEAMMC_ALL_SOURCE_FILES}
        COMMAND ${CPPCHECK}
        ${CMAKE_CURRENT_SOURCE_DIR}
        --enable=warning,performance,portability,information,missingInclude
        -i tests/Catch
        -i data/rapidjson
        --std=c++03
        --xml
        --xml-version=2
        2> cppcheck_result.xml
    )

    add_custom_target(
        ceammc_cppcheck
        DEPENDS cppcheck_result.xml
        COMMAND ${CPPCHECK_HTML}
        --file cppcheck_result.xml
        --source-dir "${CMAKE_CURRENT_SOURCE_DIR}"
        --report-dir cppcheck_html
        COMMAND ${HTML_OPEN} cppcheck_html/index.html
    )
endif()
