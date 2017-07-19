include(PdExtension)
include(../../../ext/src/ceammc_pd.cmake)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/..)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../soundtouch/include)
include_directories(${CMAKE_SOURCE_DIR}/ceammc/ext/src/lib)
#ceammc_cxx_extension(st bpm)

pd_add_extension(NAME soundtouch.bpm~
    FILES st_bpm.cpp
    INTERNAL False
    LIBRARY soundtouch
    LINK ceammc_core soundtouch)
