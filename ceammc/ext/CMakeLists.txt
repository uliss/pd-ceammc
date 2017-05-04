include(PdExtension)

add_subdirectory(src)
add_subdirectory(doc)

add_custom_target(ceammc_completion_src SOURCES ceammc_objects.txt)
# install completion
install(FILES ceammc_objects.txt DESTINATION ${PD_GUI_PLUGINS_PATH}/ceammc)
add_custom_target(ceammc_completion
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/src/lib/ext_list > ${CMAKE_CURRENT_SOURCE_DIR}/ceammc_objects.txt
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/src/lib/ext_list -v > ${CMAKE_CURRENT_SOURCE_DIR}/core_objects.txt)
