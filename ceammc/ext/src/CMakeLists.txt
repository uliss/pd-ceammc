add_library(ceammc_static STATIC ceammc.c)
set_target_properties(ceammc_static PROPERTIES
    COMPILE_FLAGS ${PD_EXTERNAL_CFLAGS})

pd_add_extension(NAME math.round FILES round.c INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.ceil FILES ceil.c INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.floor FILES floor.c INTERNAL True LINK ceammc_static)
