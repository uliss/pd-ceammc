macro(ceammc_extension_sep module name ext separator)
    pd_add_external(NAME "${module}${separator}${name}"
        FILES "${module}_${name}.${ext}"
        INTERNAL True
        LIBRARY ceammc
        LINK ceammc_core)
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
    pd_add_external(NAME "${name}" FILES "${name}.cpp" INTERNAL True LIBRARY ceammc LINK ceammc_core)
endmacro()

# adds _underscored_ target MODULE_NAME
macro(ceammc_faust_gen module name)
    # since faust 2.8.* mydsp global replacement removed
    # so we are using GNU sed
    set(options JSON VEC FTZ)
    cmake_parse_arguments(FAUST_OPT "${options}" "" "" ${ARGN})

    set(_args "")
    if(FAUST_OPT_VEC)
        list(APPEND _args "-vec" "-vs" "64")
    endif()

    if(FAUST_OPT_FTZ)
        list(APPEND _args "-ftz" "2")
    endif()

    if(FAUST_OPT_JSON)
        list(APPEND _args "-json")
    endif()

    add_custom_target("faust_${module}_${name}"
        COMMAND ${FAUST_BIN} -i
            -a ${CMAKE_SOURCE_DIR}/ceammc/faust/simple_pd_control_ext.cpp
            --class-name ${name} ${_args}
            "${CMAKE_SOURCE_DIR}/ceammc/faust/${module}_${name}.dsp"
            -o ${CMAKE_CURRENT_SOURCE_DIR}/${module}_${name}.h
        COMMAND gsed -i 's/mydsp/${name}/g' ${CMAKE_CURRENT_SOURCE_DIR}/${module}_${name}.h)
endmacro()

# adds _underscored_ target MODULE_NAME
macro(ceammc_faust_gen_obj module name)
    # since faust 2.8.* mydsp global replacement removed
    # so we are using GNU sed
    set(options JSON VEC FTZ)
    cmake_parse_arguments(FAUST_OPT "${options}" "" "" ${ARGN})

    set(_args "")
    if(FAUST_OPT_VEC)
        list(APPEND _args "-vec" "-vs" "64")
    endif()

    if(FAUST_OPT_FTZ)
        list(APPEND _args "-ftz" "2")
    endif()

    if(FAUST_OPT_JSON)
        list(APPEND _args "-json")
    endif()

    add_custom_target("faust_${module}_${name}"
        COMMAND ${FAUST_BIN} -i
            -a ${CMAKE_SOURCE_DIR}/ceammc/faust/ceammc_dsp_ext.cpp
            --class-name "${module}_${name}" ${_args}
            "${CMAKE_SOURCE_DIR}/ceammc/faust/${module}_${name}.dsp"
            -o ${CMAKE_CURRENT_SOURCE_DIR}/${module}_${name}.h
        COMMAND gsed -i 's/mydsp/${module}_${name}/g' ${CMAKE_CURRENT_SOURCE_DIR}/${module}_${name}.h)
endmacro()

# adds target "faust_MODULE_NAME" for updating faust DSP extension.
# adds extension target "MODULE_NAME~"
# file named "MODULE_NAME.dsp" should exists in ceammc/faust directory
macro(ceammc_faust_extension module name ext)
    ceammc_faust_gen(${module} ${name})
    pd_add_external(NAME "${module}.${name}~"
        FILES "${module}_${name}.cpp" INTERNAL TRUE LINK ceammc_core)
    set_target_properties("${module}.${name}~" PROPERTIES COMPILE_FLAGS "-DFAUST_MACRO")
endmacro()

function(ceammc_faust_dsp module name ext)
    ceammc_faust_gen_obj(${module} ${name})
    pd_add_external(NAME "${module}.${name}~"
        FILES "${module}_${name}.cpp" INTERNAL TRUE LINK ceammc_core)
    set_target_properties("${module}.${name}~" PROPERTIES COMPILE_FLAGS "-DFAUST_MACRO")
endfunction()
