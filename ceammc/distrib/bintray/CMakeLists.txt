configure_file(${CMAKE_CURRENT_SOURCE_DIR}/upload_bintray.sh
               ${PROJECT_BINARY_DIR}/dist/upload_bintray.sh @ONLY)

add_custom_target(bintray_scripts SOURCES upload_bintray.sh)
