include(faust)
include(ceammc_pd.cmake)
# we need 1.47.0 for boost random uniform distribution
find_package(Boost 1.47.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(lib)
include_directories(lib)
include_directories(${PROJECT_SOURCE_DIR}/ceammc/extra/CicmWrapper/Sources)
add_subdirectory(tests)
remove_definitions(-DPD_INTERNAL)
add_definitions(-DPD)

add_subdirectory(conv)
add_subdirectory(data)
add_subdirectory(dyn)
add_subdirectory(flt)
add_subdirectory(fx)
add_subdirectory(list)
add_subdirectory(math)
add_subdirectory(noise)
add_subdirectory(osc)


pd_add_extension(NAME ceammc
    FILES ceammc.cpp
    INTERNAL True
    LIBRARY ceammc
    LINK ceammc_conv ceammc_data ceammc_dyn ceammc_flt ceammc_fx ceammc_list
    ceammc_math ceammc_noise ceammc_osc cicm_static ceammc_core)

ceammc_cxx_extension(debug gensym)

###############################
# VECTOR
###############################
ceammc_cxx_extension(vector length)
ceammc_cxx_extension(vector normalize)
ceammc_cxx_extension(vector distance)
ceammc_cxx_extension(vector dot)

###############################
# TIMELINE
###############################
ceammc_cxx_tl_extension(tl transport)
ceammc_cxx_tl_extension(tl bang)
ceammc_cxx_tl_extension(tl cue)

###############################
# GUI
###############################
ceammc_cxx_extension(ui keyboard)
ceammc_cxx_extension(ui slider2d)
ceammc_cxx_extension(ui sliders)
ceammc_cxx_extension(ui knob)
ceammc_cxx_extension(ui scope~)
ceammc_cxx_extension(ui spectroscope~)
ceammc_cxx_extension(ui bpfunc)
ceammc_cxx_extension(ui display)
ceammc_cxx_extension(ui link)

###############################
# EXPERIMENTAL
###############################
if(APPLE)
ceammc_cxx_extension(exp patchdata)

ceammc_cxx_extension(ui ecanvas)

ceammc_cxx_extension(exp recursion)

ceammc_cxx_extension(exp class)
ceammc_cxx_extension(exp instance)
ceammc_cxx_extension(exp method)
ceammc_cxx_extension(exp aproperty)
ceammc_cxx_extension(exp signal~)
ceammc_cxx_extension(exp this)
ceammc_cxx_extension(exp parent)
ceammc_cxx_extension(exp vinstance)

endif()


###############################
# STREAM
###############################
ceammc_cxx_extension(reject if)
ceammc_cxx_extension(reject this)

ceammc_cxx_extension(pass if)
ceammc_cxx_extension(pass changed)
ceammc_cxx_extension(pass this)

ceammc_cxx_extension_simple(replace)

###############################
# IS
###############################
ceammc_cxx_extension__(is even)
ceammc_cxx_extension__(is odd)
ceammc_cxx_extension__(is any)
ceammc_cxx_extension__(is bang)
ceammc_cxx_extension__(is float)
ceammc_cxx_extension__(is list)
ceammc_cxx_extension__(is symbol)
ceammc_cxx_extension__(is pointer)

###############################
# TIME
###############################
# ceammc_cxx_extension(time current)

###############################
# STRING
###############################

###############################
# PATH
###############################

ceammc_extension_sep(prop "<-" cpp "")
ceammc_extension_sep(prop "->" cpp "")
pd_add_extension(NAME "msg" FILES "msg.cpp" INTERNAL True LIBRARY ceammc LINK ceammc_core)

if(UNIX)
    add_custom_command(TARGET msg COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_SONAME_FILE:msg>" "m${PD_EXTERNAL_EXTENSION}")
elseif(WIN32)
#    add_custom_command(TARGET msg COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_DLLNAME_FILE:msg>" "m${PD_EXTERNAL_EXTENSION}")
endif()

pd_add_extension(NAME test.data   FILES test_data.cpp   INTERNAL True LINK ceammc_core)
pd_add_extension(NAME test.expect FILES test_expect.cpp INTERNAL True LINK ceammc_core)
ceammc_cxx_extension(random float)
ceammc_cxx_extension(random int)
ceammc_cxx_extension(random gauss)
# symbol
ceammc_cxx_extension(symbol ends_with)


find_package(GLIB)
if(GLIB_FOUND)
    include_directories(${GLIB_INCLUDE_DIRS})

    ceammc_glib_extension(path basename)
    ceammc_glib_extension(path dirname)
    ceammc_glib_extension(path exists)
    ceammc_glib_extension(path home)
    ceammc_glib_extension(path listdir)

    ceammc_glib_extension(symbol length)

    ceammc_glib_extension(string join)
    ceammc_glib_extension(string slice)
    ceammc_glib_extension(string compare)
    ceammc_glib_extension(string split)

    ceammc_glib_extension(system hostname)
    ceammc_glib_extension(system getenv)
    ceammc_glib_extension__(is file)
endif()

ceammc_cxx_extension(system memsize)
ceammc_cxx_extension(system memused)
ceammc_cxx_extension(system screen_size)

###############################
# DSP
###############################
ceammc_faust_extension(lfo impulse simple_pd_control_ext)
ceammc_faust_extension(lfo tri simple_pd_control_ext)
ceammc_faust_extension(lfo square simple_pd_control_ext)

ceammc_faust_extension(spat pan2 simple_pd_control_ext)
ceammc_faust_extension(spat pan4 simple_pd_control_ext)

ceammc_faust_extension(env adsr simple_pd_control_ext)
ceammc_faust_extension(env ar simple_pd_control_ext)
ceammc_faust_extension(env follow simple_pd_control_ext)


ceammc_faust_extension(live capture simple_pd_control_ext)

ceammc_cxx_extension(test prop)

if(TARGET ceammc_sound)
    pd_add_extension(NAME "snd.file"
        FILES "snd_file.cpp"
        INTERNAL True
        LIBRARY ceammc
        LINK ceammc_sound ceammc_core)
endif()

file(GLOB ceammc_ext_h *.h)
add_custom_target(ceammc_lib_src SOURCES apple_rpath_fix.sh gen_lib.sh ${ceammc_ext_h})
