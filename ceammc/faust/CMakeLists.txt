file(GLOB _FAUST_FILES "*.dsp")
file(GLOB _FAUST_LIB_FILES "*.lib")
file(GLOB _FAUST_ARCH_FILES "*.cpp")
file(GLOB _FAUST_SH_FILES "*.sh")

add_custom_target(faust_files SOURCES ${_FAUST_FILES} ${_FAUST_LIB_FILES} ${_FAUST_ARCH_FILES} ${_FAUST_SH_FILES})
