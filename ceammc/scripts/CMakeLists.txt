file(GLOB _scripts "*.sh")
add_custom_target(ceammc_scripts SOURCES ${_scripts})


configure_file(vanilla_diff_src_files.sh ${PROJECT_BINARY_DIR}/scripts @ONLY)
