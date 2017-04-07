include(faust)
include(ceammc_pd.cmake)
# we need 1.47.0 for boost random uniform distribution
find_package(Boost 1.47.0 REQUIRED)
find_package(GLIB)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(lib)
include_directories(lib)
include_directories(${PROJECT_SOURCE_DIR}/ceammc/extra/CicmWrapper/Sources)
include_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(tests)
remove_definitions(-DPD_INTERNAL)
add_definitions(-DPD)

add_subdirectory(base)
add_subdirectory(conv)
add_subdirectory(data)
add_subdirectory(dyn)
add_subdirectory(env)
add_subdirectory(exp)
add_subdirectory(flt)
add_subdirectory(fx)
add_subdirectory(lfo)
add_subdirectory(list)
add_subdirectory(math)
add_subdirectory(noise)
add_subdirectory(osc)
add_subdirectory(system)
add_subdirectory(tl)
add_subdirectory(ui)
add_subdirectory(vector)

set(EXTRA_LIB "")
if(GLIB_FOUND)
    list(APPEND EXTRA_LIB ${GLIB_LIBRARIES})
endif()


pd_add_extension(NAME ceammc
    FILES ceammc.cpp
    INTERNAL True
    LIBRARY ceammc
    LINK ceammc_base ceammc_conv ceammc_data ceammc_dyn ceammc_env ceammc_flt ceammc_fx ceammc_lfo
    ceammc_list ceammc_math ceammc_noise ceammc_osc ceammc_system ceammc_tl ceammc_ui ceammc_vector
    cicm_static ceammc_core ceammc_sound ${EXTRA_LIB})

ceammc_link_fix(ceammc)

ceammc_cxx_extension(debug gensym)

###############################
# TIME
###############################
# ceammc_cxx_extension(time current)

pd_add_extension(NAME test.data   FILES test_data.cpp   INTERNAL True LINK ceammc_core)
pd_add_extension(NAME test.expect FILES test_expect.cpp INTERNAL True LINK ceammc_core)
ceammc_cxx_extension(random float)
ceammc_cxx_extension(random int)
ceammc_cxx_extension(random gauss)
# symbol
ceammc_cxx_extension(symbol ends_with)

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

    ceammc_glib_extension__(is file)
endif()

###############################
# DSP
###############################
ceammc_faust_extension(spat pan2 simple_pd_control_ext)
ceammc_faust_extension(spat pan4 simple_pd_control_ext)

ceammc_faust_extension(live capture simple_pd_control_ext)

ceammc_cxx_extension(test prop)

file(GLOB ceammc_ext_h *.h)
add_custom_target(ceammc_lib_src SOURCES apple_rpath_fix.sh gen_lib.sh ${ceammc_ext_h})
