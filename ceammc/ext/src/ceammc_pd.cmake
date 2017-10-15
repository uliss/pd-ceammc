macro(ceammc_extension_sep module name ext separator)
    pd_add_extension(NAME "${module}${separator}${name}"
        FILES "${module}_${name}.${ext}"
        INTERNAL True
        LIBRARY ceammc
        LINK cicm_static ceammc_core)
endmacro()

# adds .dotted. extension on C language: MODULE.NAME
macro(ceammc_c_extension module name)
    ceammc_extension_sep(${module} ${name} c ".")
endmacro()

# adds .dotted. extension on C++ language: MODULE.NAME
macro(ceammc_cxx_extension module name)
    ceammc_extension_sep(${module} ${name} cpp ".")
endmacro()

# adds _underscored_ extension on C++ language: MODULE_NAME
macro(ceammc_cxx_extension__ module name)
    ceammc_extension_sep(${module} ${name} cpp "_")
endmacro()

# adds simple C++ extension
macro(ceammc_cxx_extension_simple name)
    pd_add_extension(NAME "${name}" FILES "${name}.cpp" INTERNAL True LIBRARY ceammc LINK ceammc_core)
endmacro()

# adds _underscored_ target linked with *glib* library: MODULE_NAME
macro(ceammc_faust_gen module name)
    add_custom_target("faust_${module}_${name}"
        COMMAND ${FAUST_BIN} -i
            -a ${CMAKE_SOURCE_DIR}/ceammc/faust/simple_pd_control_ext.cpp
            -cn ${name}
            "${CMAKE_SOURCE_DIR}/ceammc/faust/${module}_${name}.dsp"
            -o ${CMAKE_CURRENT_SOURCE_DIR}/${module}_${name}.h)
endmacro()

# adds target "faust_MODULE_NAME" for updating faust DSP extension.
# adds extension target "MODULE_NAME~"
# file named "MODULE_NAME.dsp" should exists in ceammc/faust directory
macro(ceammc_faust_extension module name ext)
    ceammc_faust_gen(${module} ${name})
    pd_add_extension(NAME "${module}.${name}~"
        FILES "${module}_${name}.cpp" INTERNAL TRUE LINK ceammc_core)
    set_target_properties("${module}.${name}~" PROPERTIES COMPILE_FLAGS "-DFAUST_MACRO")
endmacro()
