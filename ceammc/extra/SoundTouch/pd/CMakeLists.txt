include(PdExtension)
# for m_pd.h and others
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/ceammc/ext/src/lib)

pd_add_extension(NAME soundtouch~
    FILES soundtouch_tilde.cpp
    INTERNAL False
    LIBRARY ceammc
    LINK ceammc_core soundtouch) 
