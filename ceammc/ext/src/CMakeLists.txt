add_library(ceammc_static STATIC ceammc.c)

pd_add_extension(NAME math.round FILES round.c INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.ceil FILES ceil.c INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.floor FILES floor.c INTERNAL True LINK ceammc_static)
