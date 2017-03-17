include(PdExtension)
include(../../ext/src/ceammc_pd.cmake)
# for m_pd.h and others
include_directories(${PROJECT_SOURCE_DIR}/src)

ceammc_c_extension(system colorpanel)
ceammc_c_extension(system cursor)
