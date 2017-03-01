include(faust)
include(ceammc_pd.cmake)
add_subdirectory(lib)
include_directories(lib)
include_directories(${PROJECT_SOURCE_DIR}/ceammc/extra/CicmWrapper/Sources)
add_subdirectory(tests)
remove_definitions(-DPD_INTERNAL)
add_definitions(-DPD)

add_subdirectory(math)
add_subdirectory(noise)
add_subdirectory(list)

###############################
# DATA
###############################
ceammc_cxx_extension(data fifo)
ceammc_cxx_extension(data list)

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
# BPF CONVERTERS
###############################
ceammc_cxx_extension(conv bpf2env)
ceammc_cxx_extension(conv bpf2list)
ceammc_cxx_extension(conv bpf2vline)
ceammc_cxx_extension(conv env2bpf)
ceammc_cxx_extension(conv list2bpf)
ceammc_cxx_extension(conv vline2bpf)
ceammc_cxx_extension(conv bpf2array)
ceammc_cxx_extension(conv lin2lin)

###############################
# JSON
###############################
ceammc_cxx_extension(global json)
ceammc_cxx_extension(local json)

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

###############################
# GLOBAL / LOCAL
###############################
ceammc_cxx_extension(global int)
ceammc_cxx_extension(global float)
ceammc_cxx_extension(global list)

ceammc_cxx_extension(local list)
ceammc_cxx_extension(local int)
ceammc_cxx_extension(local float)

ceammc_cxx_extension(prop get)
pd_add_extension(NAME "msg" FILES "msg.cpp" INTERNAL True LIBRARY ceammc LINK ceammc_static)

if(UNIX)
    add_custom_command(TARGET msg COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_SONAME_FILE:msg>" "m${PD_EXTERNAL_EXTENSION}")
elseif(WIN32)
#    add_custom_command(TARGET msg COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_DLLNAME_FILE:msg>" "m${PD_EXTERNAL_EXTENSION}")
endif()


include(FindLibSndFile)
if(LIBSNDFILE_FOUND)
    include_directories(${LIBSNDFILE_INCLUDE_DIRS})
#    pd_add_extension(NAME ffmpeg FILES ffmpeg.cpp
#    INTERNAL True LINK ceammc_static ${LIBSNDFILE_LIBRARIES})
endif()

# we need 1.47.0 for boost random uniform distribution
find_package(Boost 1.47.0)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    ceammc_cxx_extension(math prime)
    pd_add_extension(NAME test.data   FILES test_data.cpp   INTERNAL True LINK ceammc_static)
    pd_add_extension(NAME test.expect FILES test_expect.cpp INTERNAL True LINK ceammc_static)
    ceammc_cxx_extension(random float)
    ceammc_cxx_extension(random int)
    ceammc_cxx_extension(random gauss)

    # symbol
    ceammc_cxx_extension(symbol ends_with)
#    ceammc_cxx_extension(symbol to_upper)
endif()


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

ceammc_faust_extension(fx freqshift simple_pd_control_ext)
ceammc_faust_extension(fx pitchshift simple_pd_control_ext)

ceammc_faust_extension(flt dcblock simple_pd_control_ext)
ceammc_faust_extension(flt dcblock2 simple_pd_control_ext)
ceammc_faust_extension(flt highshelf simple_pd_control_ext)
ceammc_faust_extension(flt lowshelf simple_pd_control_ext)
ceammc_faust_extension(flt eq_peak simple_pd_control_ext)
ceammc_faust_extension(flt lpf12 simple_pd_control_ext)
ceammc_faust_extension(flt lpf24 simple_pd_control_ext)
ceammc_faust_extension(flt hpf12 simple_pd_control_ext)
ceammc_faust_extension(flt hpf24 simple_pd_control_ext)
ceammc_faust_extension(flt bpf12 simple_pd_control_ext)
ceammc_faust_extension(flt bpf24 simple_pd_control_ext)

ceammc_faust_extension(fx freeverb simple_pd_control_ext)
ceammc_faust_extension(fx freeverb2 simple_pd_control_ext)

ceammc_faust_extension(flt biquad simple_pd_control_ext)
ceammc_faust_extension(flt c_lpf simple_pd_control_ext)
ceammc_faust_extension(flt c_hpf simple_pd_control_ext)
ceammc_faust_extension(flt c_bpf simple_pd_control_ext)
ceammc_faust_extension(flt c_peak simple_pd_control_ext)
ceammc_faust_extension(flt c_lowshelf simple_pd_control_ext)
ceammc_faust_extension(flt c_highshelf simple_pd_control_ext)




ceammc_faust_extension(dyn comp simple_pd_control_ext)
ceammc_faust_extension(dyn comp2 simple_pd_control_ext)
ceammc_faust_extension(dyn gate  simple_pd_control_ext)
ceammc_faust_extension(dyn gate2 simple_pd_control_ext)
ceammc_faust_extension(dyn limit simple_pd_control_ext)
ceammc_faust_extension(dyn limit2 simple_pd_control_ext)
ceammc_faust_extension(dyn softclip simple_pd_control_ext)

ceammc_faust_extension(noise white simple_pd_control_ext)
ceammc_faust_extension(noise pink simple_pd_control_ext)
ceammc_faust_extension(noise lfreq simple_pd_control_ext)

#test!
#ceammc_faust_extension(noise lorenz simple_pd_control_ext)

ceammc_faust_extension(spat pan2 simple_pd_control_ext)
ceammc_faust_extension(spat pan4 simple_pd_control_ext)

ceammc_faust_extension(osc impulse simple_pd_control_ext)
ceammc_faust_extension(osc pulse simple_pd_control_ext)
ceammc_faust_extension(osc saw simple_pd_control_ext)
ceammc_faust_extension(osc sinfb simple_pd_control_ext)
ceammc_faust_extension(osc square simple_pd_control_ext)
ceammc_faust_extension(osc tri simple_pd_control_ext)

ceammc_faust_extension(env adsr simple_pd_control_ext)
ceammc_faust_extension(env ar simple_pd_control_ext)
ceammc_faust_extension(env follow simple_pd_control_ext)


ceammc_faust_extension(live capture simple_pd_control_ext)

ceammc_cxx_extension(test prop)
