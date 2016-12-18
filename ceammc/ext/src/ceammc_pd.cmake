macro(ceammc_extension_sep module name ext separator)
    pd_add_extension(NAME "${module}${separator}${name}"
        FILES "${module}_${name}.${ext}"
        INTERNAL True
        LIBRARY ceammc
        LINK ceammc_static)
endmacro()

# adds .dotted. extension on C language: MODULE.NAME
macro(ceammc_c_extension module name)
    ceammc_extension_sep(${module} ${name} c ".")
endmacro()

# adds .dotted. extension on C++ language: MODULE.NAME
macro(ceammc_cxx_extension module name)
    ceammc_extension_sep(${module} ${name} cpp ".")
endmacro()

# adds _underscored_ extension on C language: MODULE_NAME
macro(ceammc_c_extension__ module name)
    ceammc_extension_sep(${module} ${name} c ".")
endmacro()

# adds _underscored_ extension on C++ language: MODULE_NAME
macro(ceammc_cxx_extension__ module name)
    ceammc_extension_sep(${module} ${name} cpp "_")
endmacro()


macro(ceammc_link_fix_sep module name separator)
    if(APPLE)
        add_custom_command(TARGET "${module}${separator}${name}" POST_BUILD
            COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/apple_poco_path_fix.sh
                    $<TARGET_FILE:${module}${separator}${name}>
                    ${CMAKE_CURRENT_BINARY_DIR})
    endif()
endmacro()

macro(ceammc_glib_extension_sep module name separator)
    pd_add_extension(NAME "${module}${separator}${name}"
        FILES "${module}_${name}.cpp"
        INTERNAL True
        LINK ${GLIB_LIBRARIES})

    ceammc_link_fix_sep(${module} ${name} ${separator})
endmacro()

# adds .dotted. *glib linked* module: MODULE.NAME
macro(ceammc_glib_extension module name)
    ceammc_glib_extension_sep(${module} ${name} ".")
endmacro()

# adds _underscored_ target linked with *glib* library: MODULE_NAME
macro(ceammc_glib_extension__ module name)
    ceammc_glib_extension_sep(${module} ${name} "_")
endmacro()

# adds target "faust_MODULE_NAME" for updating faust DSP extension.
# adds extension target "MODULE_NAME~"
# file named "MODULE_NAME.dsp" should exists in ceammc/faust directory
macro(ceammc_faust_extension module name ext)
    add_custom_target("faust_${module}_${name}"
        COMMAND ${FAUST_BIN} -vec -vs 64 -i
            -a ${CMAKE_SOURCE_DIR}/ceammc/faust/${ext}.cpp
            -cn ${name}
            "${CMAKE_SOURCE_DIR}/ceammc/faust/${module}_${name}.dsp"
            -o ${CMAKE_CURRENT_SOURCE_DIR}/${module}_${name}.h)

    pd_add_extension(NAME "${module}.${name}~"
        FILES "${module}_${name}.cpp" INTERNAL TRUE)
endmacro()


macro(ceammc_cxx11_extension module name)
    ceammc_cxx_extension(${module} ${name} cpp)
    set_target_properties("${module}.${name}" PROPERTIES COMPILE_FLAGS "-std=c++11 -stdlib=libstdc++")
endmacro()

macro(ceammc_cxx_tl_extension module name)
    pd_add_extension(NAME "${module}.${name}"
      FILES "${module}_${name}.cpp"
      INTERNAL TRUE
      LIBRARY ceammc
      LINK timeline)

    set_target_properties("${module}.${name}" PROPERTIES COMPILE_FLAGS "-std=c++11")
endmacro()
