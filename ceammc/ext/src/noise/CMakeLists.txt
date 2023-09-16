set(NOISE_SOURCES mod_noise.h mod_noise.cpp)

macro(ceammc_noise_external name)
    list(APPEND NOISE_SOURCES "noise_${name}.c")
endmacro()

macro(ceammc_faust_noise_obj_ui name)
    list(APPEND NOISE_SOURCES "noise_${name}.cpp")
    ceammc_faust_gen_obj(noise ${name} JSON)
endmacro()

macro(ceammc_faust_noise_obj_ui_double name)
    list(APPEND NOISE_SOURCES "noise_${name}.cpp")
    ceammc_faust_gen_obj(noise ${name} JSON DOUBLE)
endmacro()

ceammc_noise_external(baker)
ceammc_noise_external(clifford)
ceammc_noise_external(collatz)
ceammc_noise_external(duffing)
ceammc_noise_external(ginger)
ceammc_noise_external(henon)
ceammc_noise_external(henonf)
ceammc_noise_external(henon_heilles)
ceammc_noise_external(henon_phase)
ceammc_noise_external(ikeda)
ceammc_noise_external(jong)
ceammc_noise_external(logistic)
ceammc_noise_external(lorenz)
ceammc_noise_external(lyapunov)
ceammc_noise_external(navier_stokes)
ceammc_noise_external(rossler)
ceammc_noise_external(stein)
ceammc_noise_external(torus)
ceammc_noise_external(verhulst)

ceammc_faust_noise_obj_ui_double(chua)
ceammc_faust_noise_obj_ui(colored)
ceammc_faust_noise_obj_ui(crackle)
ceammc_faust_noise_obj_ui(lfreq)
ceammc_faust_noise_obj_ui(lfreq0)
ceammc_faust_noise_obj_ui(pink)
ceammc_faust_noise_obj_ui(white)

list(APPEND NOISE_SOURCES chaos_standard.cpp chaos_gbman.cpp)

add_library(ceammc_noise STATIC ${NOISE_SOURCES})
target_include_directories(ceammc_noise
    PUBLIC ${PROJECT_SOURCE_DIR}/src
    PRIVATE
        "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>")
target_compile_definitions(ceammc_noise PUBLIC "FAUST_MACRO")
