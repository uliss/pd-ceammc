add_executable(test_ext_lang test_lang_base.h)

target_sources(test_ext_lang PRIVATE test_lang_base.cpp)
target_compile_definitions(test_ext_lang
    PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE -DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}")

target_include_directories(test_ext_lang
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/lang"
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${CATCH_INCLUDE_DIR}")

target_link_libraries(test_ext_lang
    PUBLIC
        tests_main_lib
        ceammc_lang
        ceammc_core
        puredata-core)

add_test(NAME test_ext_lang
    COMMAND test_ext_lang
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

macro(add_lang_test src)
    target_sources(test_ext_lang PRIVATE "test_lang_${src}.cpp")
endmacro()

if(WITH_LUAJIT)
    add_lang_test(luajit)
    target_link_libraries(test_ext_lang PRIVATE ${LUAJIT_LINK_LIBRARIES})
endif()
