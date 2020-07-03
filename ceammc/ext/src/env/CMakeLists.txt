set(ENV_SOURCES datatype_env.cpp
    env_concat.cpp
    env_env.cpp
    env_mix.cpp
    env_to_array.cpp
    env_to_vline.cpp
    env_tscale.cpp
    env_tshift.cpp
    env_vscale.cpp
    vline_to_env.cpp
)

macro(ceammc_env_obj_external name)
    list(APPEND ENV_SOURCES "env_${name}.cpp")
    ceammc_faust_gen_obj(env ${name})
endmacro()

ceammc_env_obj_external(adsr)
ceammc_env_obj_external(ar)
ceammc_env_obj_external(asr)
ceammc_env_obj_external(follow)
ceammc_env_obj_external(smooth)

add_library(ceammc_env STATIC mod_env.cpp ${ENV_SOURCES})
set_target_properties(ceammc_env PROPERTIES COMPILE_DEFINITIONS "FAUST_MACRO")
