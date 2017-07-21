include(PdExtension)
# for m_pd.h and others
include_directories(${PROJECT_SOURCE_DIR}/src)


pd_add_extension(NAME soundtouch~
    FILES soundtouch.cpp
    INTERNAL False
    LIBRARY ceammc
    LINK ceammc_core) 
