include_directories(Catch/single_include)
include_directories(../lib)
include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(test_atom test_atom.cpp)
target_link_libraries(test_atom ceammc_static puredata-core)
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

    set_target_properties(test_atom PROPERTIES
        COMPILE_FLAGS "--coverage"
        LINK_FLAGS "--coverage")
endif()

add_test("ceammc::Atom" test_atom)
