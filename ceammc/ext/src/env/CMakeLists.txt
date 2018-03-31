set(ENV_SOURCES datatype_env.cpp
    env_to_vline.cpp env_env.cpp env_to_array.cpp
    env_tscale.cpp env_vscale.cpp env_tshift.cpp env_mix.cpp env_concat.cpp)

macro(ceammc_env_extension name)
    list(APPEND ENV_SOURCES "env_${name}.cpp")
    ceammc_faust_gen(env ${name})
endmacro()

ceammc_env_extension(adsr)
ceammc_env_extension(asr)
ceammc_env_extension(ar)
ceammc_env_extension(follow)
ceammc_env_extension(smooth)

add_library(ceammc_env STATIC mod_env.cpp ${ENV_SOURCES})
set_target_properties(ceammc_env PROPERTIES COMPILE_DEFINITIONS "FAUST_MACRO")
if(UNIX)
    set_target_properties(ceammc_env PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
