include(faust)
include(ceammc_pd.cmake)
# we need 1.47.0 for boost random uniform distribution
find_package(Boost 1.47.0 REQUIRED)
find_package(GLIB)
include_directories(${Boost_INCLUDE_DIRS})

set(CEAMMC_EXT_LINK_LIBRARIES ceammc_base ceammc_conv ceammc_data ceammc_dyn ceammc_env ceammc_flt ceammc_fx ceammc_lfo
    ceammc_list ceammc_math ceammc_misc ceammc_noise ceammc_osc ceammc_path ceammc_random ceammc_spat
    ceammc_symbol ceammc_system ceammc_tl ceammc_ui ceammc_vector
    cicm_static ceammc_core ceammc_sound)

if(GLIB_FOUND)
    list(APPEND CEAMMC_EXT_LINK_LIBRARIES ${GLIB_LIBRARIES})
endif()

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
add_subdirectory(misc)
add_subdirectory(noise)
add_subdirectory(osc)
add_subdirectory(path)
add_subdirectory(random)
add_subdirectory(spat)
add_subdirectory(symbol)
add_subdirectory(system)
add_subdirectory(tl)
add_subdirectory(ui)
add_subdirectory(vector)

pd_add_extension(NAME ceammc
    FILES ceammc.cpp mod_init.cpp
    INTERNAL True
    LIBRARY ceammc
    LINK ${CEAMMC_EXT_LINK_LIBRARIES})

ceammc_link_fix(ceammc)

add_custom_target(ceammc_lib_src SOURCES apple_rpath_fix.sh gen_lib.sh)


set(CEAMMC_COMPLETION "${CMAKE_CURRENT_SOURCE_DIR}/ceammc_completion.txt")
add_custom_target(ceammc_completion
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/lib/ext_list > ${CEAMMC_COMPLETION})
