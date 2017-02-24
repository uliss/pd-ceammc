file(GLOB _FAUST_FILES "*.dsp")
file(GLOB _FAUST_LIB_FILES "*.lib")
file(GLOB _FAUST_ARCH_FILES "*.cpp")

add_custom_target(faust_files SOURCES ${_FAUST_FILES} ${_FAUST_LIB_FILES} ${_FAUST_ARCH_FILES})
