include_directories(${CMAKE_SOURCE_DIR}/src)

set(FLT_SOURCES
    flt_common.cpp
    flt_freqz.cpp
    flt_freqz_tilde.cpp
)

macro(ceammc_flt_obj name)
    list(APPEND FLT_SOURCES "flt_${name}.h" "flt_${name}.cpp")
    ceammc_faust_gen_obj(flt ${name})
endmacro()

macro(ceammc_flt_obj_json name)
    list(APPEND FLT_SOURCES "flt_${name}.h" "flt_${name}.cpp")
    ceammc_faust_gen_obj(flt ${name} JSON)
endmacro()

macro(ceammc_flt_obj_json_double name)
    list(APPEND FLT_SOURCES "flt_${name}.h" "flt_${name}.cpp")
    ceammc_faust_gen_obj(flt ${name} JSON DOUBLE)
endmacro()

ceammc_flt_obj(biquad)
ceammc_flt_obj(dcblock)
ceammc_flt_obj(dcblock2)

ceammc_flt_obj_json_double(bpf12)
ceammc_flt_obj_json_double(bpf24)
ceammc_flt_obj_json_double(moog_vcf)

ceammc_flt_obj_json(c_bpf)
ceammc_flt_obj_json(c_highshelf)
ceammc_flt_obj_json(c_hpf)
ceammc_flt_obj_json(c_lowshelf)
ceammc_flt_obj_json(c_lpf)
ceammc_flt_obj_json(c_notch)
ceammc_flt_obj_json(c_peak)
ceammc_flt_obj_json(eq10)
ceammc_flt_obj_json(eq_peak)
ceammc_flt_obj_json(eq_peak_cq)
ceammc_flt_obj_json(fb_comb)
ceammc_flt_obj_json(ff_comb)
ceammc_flt_obj_json(highshelf)
ceammc_flt_obj_json(hpf12)
ceammc_flt_obj_json(hpf24)
ceammc_flt_obj_json(lowshelf)
ceammc_flt_obj_json(lpf12)
ceammc_flt_obj_json(lpf24)
ceammc_flt_obj_json(notch)
ceammc_flt_obj_json(resonbp)

list(APPEND FLT_SOURCES "flt_median.h" "flt_median.cpp")

add_library(ceammc_flt STATIC
    mod_flt.h
    mod_flt.cpp ${FLT_SOURCES}
    flt_calc_biquad.cpp
    flt_hpf2biquad.cpp
    flt_lpf2biquad.cpp
    flt_notch2biquad.cpp
    flt_pole2biquad.cpp
)
set_target_properties(ceammc_flt PROPERTIES COMPILE_DEFINITIONS "FAUST_MACRO")
