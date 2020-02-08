include(PdExternal)

add_subdirectory(src)
add_subdirectory(doc)

if(ENABLE_TESTS)
    add_subdirectory(tests)
endif()

add_subdirectory(abstractions)
add_subdirectory(class-wrapper)

if(WITH_BENCHMARK)
    if(WITH_COVERAGE)
        message(STATUS "Coverage and benchmarks are not supported together")
    else()
        add_subdirectory(benchmark)
    endif()
endif()

add_custom_target(ceammc_completion_src SOURCES ceammc_objects.txt extra_objects.txt)
# install completion
install(FILES ceammc_objects.txt DESTINATION ${PD_GUI_PLUGINS_PATH}/ceammc/custom_completions)
install(FILES extra_objects.txt DESTINATION ${PD_GUI_PLUGINS_PATH}/ceammc/custom_completions)
add_custom_target(ceammc_completion
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/src/lib/ext_list > ${CMAKE_CURRENT_SOURCE_DIR}/ceammc_objects.txt
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/src/lib/ext_list -v > ${CMAKE_CURRENT_SOURCE_DIR}/core_objects.txt
    COMMAND cat ${CMAKE_SOURCE_DIR}/ceammc/extra/extra_objects.txt > ${CMAKE_CURRENT_SOURCE_DIR}/extra_objects.txt
    COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/abstractions/abstractions.txt >> ${CMAKE_CURRENT_SOURCE_DIR}/extra_objects.txt)
