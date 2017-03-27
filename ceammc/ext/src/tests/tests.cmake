macro(ceammc_add_test title name)
    add_executable(${name} "${name}.cpp")
    target_link_libraries(${name} tests_main_lib ceammc_core puredata-core)
    add_test(${title} ${name})

    if(${WITH_COVERAGE})
        set_target_properties(${name} PROPERTIES COMPILE_FLAGS "--coverage" LINK_FLAGS "--coverage")
    endif()
endmacro()

macro(ceammc_add_test_linked)
    set(_OPTIONS_ARGS)
    set(_ONE_VALUE_ARGS TITLE NAME)
    set(_MULTI_VALUE_ARGS LINK)

    cmake_parse_arguments(_TEST "${_OPTIONS_ARGS}" "${_ONE_VALUE_ARGS}" "${_MULTI_VALUE_ARGS}" ${ARGN})

    set(name ${_TEST_NAME})
    set(title ${_TEST_TITLE})
    add_executable(${name} "${name}.cpp")
    target_link_libraries(${name} ${_TEST_LINK} tests_main_lib)
    add_test(${title} ${name})

    if(${WITH_COVERAGE})
        set_target_properties(${name} PROPERTIES COMPILE_FLAGS "--coverage" LINK_FLAGS "--coverage")
    endif()
endmacro()

macro(ceammc_add_extension_test name extpath)
    set(_target "test_${name}")
    add_executable(${_target} "${_target}.cpp" ${extpath})
    target_link_libraries(${_target} tests_main_lib puredata-core ceammc_core puredata-core ceammc_sound)
    add_test("Extension::${name}" ${_target})

    if(${WITH_COVERAGE})
        set_target_properties(${_target} PROPERTIES COMPILE_FLAGS "--coverage" LINK_FLAGS "--coverage")
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
