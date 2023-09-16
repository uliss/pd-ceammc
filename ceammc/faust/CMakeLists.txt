file(GLOB _FAUST_FILES "*.dsp")
file(GLOB _FAUST_LIB_FILES "*.lib")
file(GLOB _FAUST_ARCH_FILES "*.cpp")
file(GLOB _FAUST_SH_FILES "*.sh")

add_custom_target(faust_files SOURCES ${_FAUST_FILES} ${_FAUST_LIB_FILES} ${_FAUST_ARCH_FILES} ${_FAUST_SH_FILES})

add_custom_target("faust_update_headers"
    COMMAND ${FAUST_BIN} -i
        -a ${CMAKE_SOURCE_DIR}/ceammc/faust/faust_arch_headers_ui.cpp
        --inline-architecture-files
        --class-name CEAMMC_FAUST_ARCH_HEADERS
        --super-class-name CEAMMC_FAUST_ARCH_HEADERS_DUMMY
        --light-mode
        "${CMAKE_SOURCE_DIR}/ceammc/faust/arch_dummy.dsp"
        -o ${CMAKE_SOURCE_DIR}/ceammc/ext/src/lib/faust_common_ui.h
)
