set(TL_SOURCES "")

macro(ceammc_tl_extension name)
     list(APPEND TL_SOURCES "tl_${name}.cpp")
endmacro()

ceammc_tl_extension(transport)
ceammc_tl_extension(bang)
ceammc_tl_extension(cue)


add_library(ceammc_tl STATIC mod_tl.h mod_tl.cpp ${TL_SOURCES})
if(UNIX)
    set_target_properties(ceammc_tl PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
