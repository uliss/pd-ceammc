set(FAUST_UI_DIR "${PROJECT_SOURCE_DIR}/ceammc/faust")
set(ABSTRACTIONS_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/faust2ui.sh
    ${CMAKE_CURRENT_BINARY_DIR}/faust2ui.sh @ONLY)

install(FILES abstraction_objects.txt DESTINATION ${PD_GUI_PLUGINS_PATH}/ceammc/custom_completions)

file(GLOB ABSTRACTION_FILES "*.pd")
# install all abstractions files
foreach(_abs_file ${ABSTRACTION_FILES})
    install(FILES "${_abs_file}"
        DESTINATION "${PD_DOC_INSTALL_PATH}/extra/ceammc"
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
endforeach()

add_custom_target("faust_ui"
        COMMAND "${CMAKE_CURRENT_BINARY_DIR}/faust2ui.sh"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
