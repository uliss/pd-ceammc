include(ceammc_pd.cmake)

#add_subdirectory(${PROJECT_SOURCE_DIR}/src/lib)
#include_directories(${PROJECT_SOURCE_DIR}/ceammc/extra/CicmWrapper/Sources)

#add_subdirectory(../lib)
#include_directories(../lib)
#include_directories(${PROJECT_SOURCE_DIR}/ceammc/extra/CicmWrapper/Sources)

if(APPLE)
ceammc_cxx_extension(midi in)
ceammc_cxx_extension(midi out)
ceammc_cxx_extension(midi listin)
ceammc_cxx_extension(midi listout)
endif()