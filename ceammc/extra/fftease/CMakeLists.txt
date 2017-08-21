include(PdExtension)
include(../../ext/src/ceammc_pd.cmake)
# for m_pd.h and others
include_directories(${PROJECT_SOURCE_DIR}/src)

add_library(fftease_static STATIC 
    MSPd.h bashfest_dsp.c bashfest_helper.c convert.c bloscbank.c ellipse.c fftease_setup.c 
    fold.c leanconvert.c leanunconvert.c makewindows.c 
    from_msp.c fft.c fft4.c qsortE.c power_of_two.c overlapadd.c)

function(fftease_external name)
    pd_add_extension(NAME ${name}
        FILES ${name}.c
        INTERNAL True
        LIBRARY fftease
        LINK fftease_static
        #HELP_FILES ${name}-help.pd
        )
endfunction()
