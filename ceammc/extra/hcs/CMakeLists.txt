include(PdExternal)
include(../../ext/src/ceammc_pd.cmake)
# for m_pd.h and others
include_directories(${PROJECT_SOURCE_DIR}/src)

ceammc_c_extension(system colorpanel)

file(GLOB _hcs_tcl_files "*.tcl")
file(GLOB _hcs_help_files "*.pd")
add_custom_target(hcs_misc_files SOURCES ${_hcs_tcl_files} ${_hcs_help_files})

install(FILES ${_hcs_tcl_files}  ${_hcs_help_files}
    DESTINATION ${PD_INTERNAL_EXT_INSTALL_PATH}/ceammc)
