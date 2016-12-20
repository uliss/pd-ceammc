macro(ceammc_add_test title name)
    add_executable(${name} "${name}.cpp")
    target_link_libraries(${name} ceammc_static puredata-core)
    add_test(${title} ${name})

    if(${WITH_COVERAGE})
        set_target_properties(${name} PROPERTIES  COMPILE_FLAGS "--coverage" LINK_FLAGS "--coverage")
    endif()
endmacro()

if(${WITH_COVERAGE})
    find_program(LCOV NAMES lcov PATHS /usr/bin /usr/local/bin)
    if(LCOV)
        message(STATUS "lcov found: ${LCOV}")
        set(GCOV "/usr/local/bin/gcov-6")
        add_custom_target(coverage
            COMMAND ${LCOV}
                --gcov-tool=${GCOV}
                --directory "${CMAKE_CURRENT_BINARY_DIR}/.."
                --capture
                --output-file coverage.info
            COMMAND ${LCOV}
                --remove coverage.info 'tests/Catch/*' '/usr/*' 'src/m_pd.h'
                --output-file coverage.info
            COMMAND ${LCOV}
                --list coverage.info)

        add_custom_target(coverage_report
            COMMAND genhtml --output-directory ${CMAKE_BINARY_DIR}/coverage coverage.info
            COMMAND open ${CMAKE_BINARY_DIR}/coverage/index.html)
    endif()
endif()
