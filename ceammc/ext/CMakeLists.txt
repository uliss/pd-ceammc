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

set(CAT ${CMAKE_COMMAND} -E cat)

add_custom_target(ceammc_completion
    COMMAND $<TARGET_FILE:ext_list> > ${CMAKE_CURRENT_SOURCE_DIR}/ceammc_objects.txt
    COMMAND $<TARGET_FILE:ext_list> -v > ${CMAKE_CURRENT_SOURCE_DIR}/core_objects.txt
    COMMAND ${CAT} ${CMAKE_CURRENT_SOURCE_DIR}/src/lang/lang_extra_objects.txt >> ${CMAKE_CURRENT_SOURCE_DIR}/ceammc_objects.txt
    COMMAND ${CAT} ${PROJECT_SOURCE_DIR}/ceammc/extra/extra_objects.txt > ${CMAKE_CURRENT_SOURCE_DIR}/extra_objects.txt
    COMMAND ${CAT} ${CMAKE_CURRENT_SOURCE_DIR}/abstractions/abstractions.txt >> ${CMAKE_CURRENT_SOURCE_DIR}/extra_objects.txt)
