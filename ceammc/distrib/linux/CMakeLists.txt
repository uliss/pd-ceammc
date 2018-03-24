file(GLOB _BUILD_SCRIPTS "*.sh")
add_custom_target(ceammc_distrib_linux_sources SOURCES ${_BUILD_SCRIPTS})
