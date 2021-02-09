add_library(ceammc_lang STATIC mod_lang.cpp)

if(WITH_FAUST)
    target_sources(ceammc_lang PRIVATE
        lang_faust_tilde.cpp
        lang_faust_ui_tilde.cpp
    )
    target_compile_definitions(ceammc_lang PRIVATE WITH_FAUST)
    target_include_directories(ceammc_lang PRIVATE ${PROJECT_SOURCE_DIR}/ceammc/extra/faust)
    target_include_directories(ceammc_lang PRIVATE ${PROJECT_SOURCE_DIR}/ceammc/extra/faust/faust/architecture)
    target_link_libraries(ceammc_lang PRIVATE ceammc_faust)
else()
    target_sources(ceammc_lang PRIVATE lang_faust_tilde_stub.cpp)
endif()
