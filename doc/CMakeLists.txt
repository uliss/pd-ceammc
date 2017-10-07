install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DESTINATION ${PD_DOC_INSTALL_PATH}
    PATTERN "CMake*" EXCLUDE)

file(GLOB_RECURSE _DOC_FILES "*.htm" "*.css")
add_custom_target(_doc_files SOURCES ${_DOC_FILES})
