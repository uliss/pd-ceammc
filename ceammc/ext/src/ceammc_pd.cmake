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
macro(ceammc_faust_gen_obj module name)
    set(options JSON VEC VS FTZ OCPP DOUBLE)
    set(list_opts INCLUDES)
    cmake_parse_arguments(FAUST_OPT "${options}" "" "${list_opts}" ${ARGN})

    set(_args "")
    if(FAUST_OPT_VEC)
        list(APPEND _args "-vec" "-vs" "64")
    endif()

    if(FAUST_OPT_VS)
        list(APPEND _args "-vs" "16")
    endif()

    if(FAUST_OPT_FTZ)
        list(APPEND _args "-ftz" "2")
    endif()

    if(FAUST_OPT_JSON)
        list(APPEND _args "-json")
    endif()

    if(FAUST_OPT_OCPP)
        list(APPEND _args "-lang" "ocpp")
    endif()

    if(FAUST_OPT_DOUBLE)
        list(APPEND _args "--double-precision-floats")
    endif()

    if(FAUST_OPT_INCLUDES)
        foreach(dir ${FAUST_OPT_INCLUDES})
            list(APPEND _args "-I" ${dir})
        endforeach()
    endif()

    add_custom_target("faust_${module}_${name}"
        COMMAND ${FAUST_BIN} -i
            -a ${CMAKE_SOURCE_DIR}/ceammc/faust/ceammc_dsp_ext.cpp
            --class-name "${module}_${name}"
            --super-class-name "${module}_${name}_dsp"
            ${_args}
            "${CMAKE_SOURCE_DIR}/ceammc/faust/${module}_${name}.dsp"
            -o ${CMAKE_CURRENT_SOURCE_DIR}/${module}_${name}.h)

    set(CEAMMC_FAUST_TARGETS "${CEAMMC_FAUST_TARGETS};faust_${module}_${name}" CACHE INTERNAL "")
endmacro()
