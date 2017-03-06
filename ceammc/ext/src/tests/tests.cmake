macro(ceammc_add_test title name)
    add_executable(${name} "${name}.cpp")
    target_link_libraries(${name} ceammc_static puredata-core ceammc_timeline)
    add_test(${title} ${name})

    if(${WITH_COVERAGE})
        set_target_properties(${name} PROPERTIES COMPILE_FLAGS "--coverage" LINK_FLAGS "--coverage")
    endif()
endmacro()

macro(ceammc_add_extension_test name extpath)
    add_executable("test_${name}" "test_${name}.cpp" ${extpath})
    target_link_libraries("test_${name}" ceammc_static puredata-core ceammc_timeline)
    add_test("Extension::${name}" "test_${name}")

    if(${WITH_COVERAGE})
        set_target_properties(${name} PROPERTIES COMPILE_FLAGS "--coverage" LINK_FLAGS "--coverage")
    endif()
endmacro()

if(${WITH_COVERAGE})
    find_program(LCOV NAMES lcov PATHS /usr/bin /usr/local/bin)
    find_program (GCOV NAMES gcov-5 gcov-6 gcov PATHS /usr/bin /usr/local/bin)

    if(LCOV AND GCOV)
        message(STATUS "lcov found: ${LCOV}")
        message(STATUS "gcov found: ${GCOV}")
        add_custom_target(coverage
            COMMAND ${LCOV}
                --gcov-tool=${GCOV}
                --directory "${CMAKE_CURRENT_BINARY_DIR}/.."
                --capture
                --output-file coverage.info
            COMMAND ${LCOV}
                --remove coverage.info 'tests/Catch/*' '/usr/*' '/Applications/Xcode.app/*' 'src/m_pd.h'
                --output-file coverage.info
            COMMAND ${LCOV}
                --list coverage.info)

        add_custom_target(coverage_report
            COMMAND genhtml --output-directory ${CMAKE_BINARY_DIR}/coverage coverage.info
            COMMAND open ${CMAKE_BINARY_DIR}/coverage/index.html)
    endif()
endif()
