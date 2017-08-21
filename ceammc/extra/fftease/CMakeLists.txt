include(PdExtension)
include(../../ext/src/ceammc_pd.cmake)
# for m_pd.h and others
include_directories(${PROJECT_SOURCE_DIR}/src)

add_library(fftease_static STATIC 
    fftease/MSPd.h
    fftease/PenroseOscil.c
    fftease/PenroseOscil.h
    fftease/PenroseRand.c
    fftease/PenroseRand.h
    fftease/bloscbank.c
    fftease/convert.c
    fftease/fft.c
    fftease/fft4.c
    fftease/fftease.h
    fftease/fftease_setup.c
    fftease/fftease_utilities.c
    fftease/fold.c
    fftease/leanconvert.c
    fftease/leanunconvert.c
    fftease/legacy.c
    fftease/limit_fftsize.c
    fftease/limited_oscbank.c
    fftease/makewindows.c
    fftease/oscbank.c
    fftease/overlapadd.c
    fftease/power_of_two.c
    fftease/qsortE.c
    fftease/unconvert.c)

function(fftease_external name)
    pd_add_extension(NAME ${name}
        FILES fftease/${name}.c
        INTERNAL True
        LIBRARY fftease
        LINK fftease_static
        )
endfunction()


fftease_external(bthresher~)
fftease_external(burrow~)
fftease_external(cavoc27~)
fftease_external(cavoc~)
fftease_external(centerring~)
fftease_external(codepend~)
fftease_external(cross~)
fftease_external(dentist~)
fftease_external(disarrain~)
fftease_external(disarray~)
fftease_external(drown~)
fftease_external(enrich~)
fftease_external(ether~)
fftease_external(leaker~)
fftease_external(mindwarp~)
fftease_external(morphine~)
fftease_external(multyq~)
fftease_external(pileup~)
fftease_external(pvcompand~)
fftease_external(pvgrain~)
fftease_external(pvharm~)
fftease_external(pvoc~)
fftease_external(pvtuner~)
fftease_external(pvwarpb~)
fftease_external(pvwarp~)
fftease_external(reanimator~)
fftease_external(resent~)
fftease_external(residency_buffer~)
fftease_external(residency~)
fftease_external(schmear~)
fftease_external(scrape~)
fftease_external(shapee~)
fftease_external(swinger~)
fftease_external(taint~)
fftease_external(thresher~)
fftease_external(vacancy~)
fftease_external(xsyn~)

