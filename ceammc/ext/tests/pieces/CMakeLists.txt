add_executable(test_ext_pieces test_pieces_base.h)

target_compile_definitions(test_ext_pieces
    PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE -DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}"
)

target_include_directories(test_ext_pieces
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/pieces"
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${CATCH_INCLUDE_DIR}"
        "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>"
)

target_link_libraries(test_ext_pieces PUBLIC
    tests_main_lib
    ceammc_pieces
    ceammc_core
    puredata-core)

add_test(NAME test_ext_pieces
    COMMAND test_ext_pieces
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

target_sources(test_ext_pieces PRIVATE test_stokhausen_solo.cpp)
