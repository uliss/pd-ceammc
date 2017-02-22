
include(ceammc_pd.cmake)

#add_subdirectory(${PROJECT_SOURCE_DIR}/src/lib)
#include_directories(${PROJECT_SOURCE_DIR}/ceammc/extra/CicmWrapper/Sources)

#add_subdirectory(../lib)
#include_directories(../lib)
#include_directories(${PROJECT_SOURCE_DIR}/ceammc/extra/CicmWrapper/Sources)

ceammc_c_extension(noise baker)
ceammc_c_extension(noise clifford)
ceammc_c_extension(noise henon_heilles)
ceammc_c_extension(noise navier_stokes)
ceammc_c_extension(noise lorenz)