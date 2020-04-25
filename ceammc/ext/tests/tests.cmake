find_program(WINE_EXE NAMES wine PATHS /usr/bin /usr/local/bin)
if(WINE_EXE)
    message(STATUS "WINE found: ${WINE_EXE}")
endif()

# wine mingw32 tests
if(MINGW AND WINE_EXE)
    add_custom_target(test_wine_dll_copy ALL
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/src/libpuredata-core.dll
        ${CMAKE_CURRENT_BINARY_DIR}/libpuredata-core.dll

        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/ceammc/ext/src/lib/libceammc_core.dll
        ${CMAKE_CURRENT_BINARY_DIR}/libceammc_core.dll

        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/ceammc/ext/src/ceammc.dll
        ${CMAKE_CURRENT_BINARY_DIR}/ceammc.dll)
endif()

function(set_test_command title exec_path)
    if(MINGW AND WINE_EXE)
        add_test(NAME ${title} COMMAND ${WINE_EXE} ${exec_path})
    else()
        if(${WITH_COVERAGE})
#            set(EXEC ${CMAKE_CURRENT_SOURCE_DIR}/test_runner.sh)
#            add_test(NAME ${title}
#                COMMAND ${EXEC} ${exec_path}
#                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

            add_test(NAME ${title} COMMAND ${exec_path})
        else()
            add_test(NAME ${title} COMMAND ${exec_path})
        endif()
    endif()
endfunction()

macro(ceammc_add_core_test title name)
    add_executable(${name} "${name}.cpp")
    target_link_libraries(${name} tests_main_lib ceammc_core puredata-core)
    set_test_command(${title} ${name})
endmacro()

macro(ceammc_add_test title name)
    add_executable(${name} "${name}.cpp")
    target_link_libraries(${name} tests_main_lib ceammc_base ceammc_core puredata-core)
    set_test_command(${title} ${name})
endmacro()

macro(ceammc_add_test_linked)
    set(_OPTIONS_ARGS)
    set(_ONE_VALUE_ARGS TITLE NAME)
    set(_MULTI_VALUE_ARGS LINK SRC INCLUDE_DIRECTORIES)

    cmake_parse_arguments(_TEST "${_OPTIONS_ARGS}" "${_ONE_VALUE_ARGS}" "${_MULTI_VALUE_ARGS}" ${ARGN})

    set(name ${_TEST_NAME})
    set(title ${_TEST_TITLE})
    add_executable(${name} "${name}.cpp" ${_TEST_SRC})
    target_link_libraries(${name} ${_TEST_LINK} tests_main_lib ceammc_core puredata-core)
    target_include_directories(${name} PUBLIC ${_TEST_INCLUDE_DIRECTORIES})
    set_test_command(${title} ${name})
endmacro()

macro(ceammc_add_extension_test name extpath)
    set(_target "test_${name}")
    add_executable(${_target} "${_target}.cpp" ${extpath})
    target_link_libraries(${_target}
        tests_main_lib puredata-core ceammc_core puredata-core)
    set(_exec_cmd ${_target})
    set_test_command("Extension::${name}" ${_exec_cmd})
endmacro()

macro(ceammc_external_test external name)
    set(_target "test_ext_${external}_${name}")
    add_executable(${_target} "${_target}.cpp")
    target_compile_definitions(${_target} PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")
    target_include_directories(${_target} PRIVATE ${PROJECT_SOURCE_DIR}/ceammc/ext/src/${external})
    # library repeats are done to make mingw linker happy
    target_link_libraries(${_target} PUBLIC
        tests_main_lib puredata-core
        "ceammc_${external}" ceammc_array ceammc_data ceammc_string ceammc_core puredata-core)
    set(_exec_cmd ${_target})
    set_test_command("[${external}.${name}]" ${_exec_cmd})
endmacro()

if(${WITH_COVERAGE})
    if(APPLE)
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

        find_program (LLVM_COV NAMES llvm-cov
            PATHS /Library/Developer/CommandLineTools/usr/bin
                  /usr/bin
                  /usr/local/bin)

        find_program(GCOVR NAMES gcovr PATHS /usr/bin /usr/local/bin)

        if(LLVM_COV AND GCOVR)
            message(STATUS "llvm-cov found: ${LLVM_COV}")
            message(STATUS "gcovr found: ${GCOVR}")

            add_custom_target(coverage
                COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/llvm_cov_gen.sh
                    "${LLVM_COV}" "${CMAKE_CURRENT_BINARY_DIR}/../src")

            add_custom_target(coverage_report
                COMMAND mkdir -p ${CMAKE_BINARY_DIR}/coverage
                COMMAND ${GCOVR} --html --html-details
                    --output "${CMAKE_BINARY_DIR}/coverage/index.html"
                    --exclude-directories "lib/utf8rewind"
                    --exclude-directories "lib/json"
                    --sort-percentage
#                    --use-gcov-files
#                    --verbose
                    --object-directory "${CMAKE_BINARY_DIR}"
                    --root "${CMAKE_SOURCE_DIR}/ceammc/ext/src"
                COMMAND open ${CMAKE_BINARY_DIR}/coverage/index.html)
        endif()
    else()
        find_program(LCOV NAMES lcov PATHS /usr/bin /usr/local/bin)
        find_program (GCOV NAMES gcov-5 gcov-6 gcov-7 gcov PATHS /usr/bin /usr/local/bin)

        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

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
                    --remove coverage.info 'ceammc/ext/src/tests/Catch/*' '/usr/*' '/Applications/Xcode.app/*' 'src/m_pd.h'
                    --output-file coverage.info
                COMMAND ${LCOV}
                    --list coverage.info)

            add_custom_target(coverage_report
                COMMAND genhtml --output-directory ${CMAKE_BINARY_DIR}/coverage coverage.info
                COMMAND open ${CMAKE_BINARY_DIR}/coverage/index.html)
        endif()
    endif()
endif()
