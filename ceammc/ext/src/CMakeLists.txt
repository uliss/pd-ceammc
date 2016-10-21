add_library(ceammc_static STATIC ceammc.c)
set_target_properties(ceammc_static PROPERTIES
    COMPILE_FLAGS ${PD_EXTERNAL_CFLAGS})

add_custom_target(ceammc_generate_math
    COMMAND ${CMAKE_SOURCE_DIR}/ceammc/py/generate_math.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

pd_add_extension(NAME math.abs   FILES math_abs.c   INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.acos  FILES math_acos.c  INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.acosh FILES math_acosh.c INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.asin  FILES math_asin.c  INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.asinh FILES math_asinh.c INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.atan  FILES math_atan.c  INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.atanh FILES math_atanh.c INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.cbrt  FILES math_cbrt.c  INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.ceil  FILES math_ceil.c  INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.cos   FILES math_cos.c   INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.cosh  FILES math_cosh.c  INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.exp   FILES math_exp.c   INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.exp2  FILES math_exp2.c  INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.floor FILES math_floor.c INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.log   FILES math_log.c   INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.log10 FILES math_log10.c INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.log2  FILES math_log2.c  INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.round FILES math_round.c INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.sin   FILES math_sin.c   INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.sinh  FILES math_sinh.c  INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.sqrt  FILES math_sqrt.c  INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.tan   FILES math_tan.c   INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.tanh  FILES math_tanh.c  INTERNAL True LINK ceammc_static)

pd_add_extension(NAME math.pi    FILES math_pi.c    INTERNAL True)
pd_add_extension(NAME math.e     FILES math_e.c     INTERNAL True)
pd_add_extension(NAME math.nan   FILES math_nan.c   INTERNAL True)
pd_add_extension(NAME math.inf   FILES math_inf.c   INTERNAL True)

pd_add_extension(NAME math.reciprocal  FILES math_reciprocal.c INTERNAL True LINK ceammc_static)
pd_add_extension(NAME math.sign        FILES math_sign.c       INTERNAL True LINK ceammc_static)

pd_add_extension(NAME list.length FILES list_length.c INTERNAL True)
pd_add_extension(NAME list.separate FILES list_separate.c INTERNAL True)

#pd_add_extension(NAME path.basename FILES path_basename.cpp INTERNAL True LINK ceammc_static)
#pd_add_extension(NAME path.exists FILES path_exists.cpp INTERNAL True LINK ceammc_static)

include(FindLibSndFile)
if(LIBSNDFILE_FOUND)
    message(STATUS "FOUNDDDD")
    include_directories(${LIBSNDFILE_INCLUDE_DIRS})
#    pd_add_extension(NAME ffmpeg FILES ffmpeg.cpp INTERNAL True LINK ceammc_static ${LIBSNDFILE_LIBRARIES})
endif()

