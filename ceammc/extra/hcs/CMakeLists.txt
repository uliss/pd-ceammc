include(PdExtension)
include(../../ext/src/ceammc_pd.cmake)
# for m_pd.h and others
include_directories(${PROJECT_SOURCE_DIR}/src)

#pd_add_extension(NAME system.cursor
#    FILES system.cursor.cpp
#    INTERNAL True
#    LIBRARY ceammc
#    )

#pd_add_extension(NAME system.colorpanel
#    FILES system.colorpanel.cpp
#    INTERNAL True
#    LIBRARY ceammc
#    )

ceammc_c_extension(system colorpanel)
ceammc_c_extension(system cursor)
