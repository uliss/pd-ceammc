add_executable(dylibbundler
    macdylibbundler/src/Dependency.cpp
    macdylibbundler/src/DylibBundler.cpp
    macdylibbundler/src/main.cpp
    macdylibbundler/src/Settings.cpp
    macdylibbundler/src/Utils.cpp)

file(GLOB _BUILD_SCRIPTS "*.sh")
add_custom_target(ceammc_distrib_mac_sources SOURCES ${_BUILD_SCRIPTS})
