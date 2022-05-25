add_library(ceammc_net STATIC mod_net.cpp net_host.cpp)

# liblo
find_package(liblo)
if(LIBLO_FOUND)
    target_sources(ceammc_net PRIVATE net_osc.cpp)
    target_include_directories(ceammc_net PRIVATE ${LIBLO_INCLUDE_DIRS})
    target_link_libraries(ceammc_net PUBLIC ${LIBLO_LIBRARIES})
endif()

target_include_directories(ceammc_net
    PUBLIC
        ${PROJECT_SOURCE_DIR}/ceammc/extra/readerwriterqueue # for readerwriterqueue
    PRIVATE
        ${PROJECT_BINARY_DIR} # for config.h
)

