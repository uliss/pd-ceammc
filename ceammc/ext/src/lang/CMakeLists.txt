add_library(ceammc_lang STATIC mod_lang.cpp)
target_include_directories(ceammc_lang
    PRIVATE "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>")

if(WITH_FAUST)
    pd_add_external(NAME "lang.faust~"
        FILES lang_faust_tilde.cpp
        INTERNAL True
        LIBRARY ceammc
        LINK ceammc ceammc_faust ceammc_core)
    target_compile_definitions(lang.faust_tilde PRIVATE WITH_FAUST)
    target_include_directories(lang.faust_tilde
        PRIVATE
            "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>"
            ${PROJECT_BINARY_DIR} # for config.h
            ${PROJECT_SOURCE_DIR}/ceammc/extra/faust
            ${PROJECT_SOURCE_DIR}/ceammc/extra/faust/faust/architecture)

    pd_add_external(NAME "ui.faust~"
        FILES lang_faust_ui_tilde.cpp lang_faust_tilde.cpp
        INTERNAL True
        LIBRARY ceammc
        LINK ceammc lang.faust_tilde ceammc_faust ceammc_nui ceammc_core)
    target_compile_definitions(ui.faust_tilde PRIVATE WITH_FAUST)
    target_include_directories(ui.faust_tilde
        PRIVATE
            "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>"
            ${PROJECT_BINARY_DIR} # for config.h
            ${PROJECT_SOURCE_DIR}/ceammc/extra/faust
            ${PROJECT_SOURCE_DIR}/ceammc/extra/faust/faust/architecture)
else()
    pd_add_external(NAME "lang.faust~"
        FILES lang_faust_tilde_stub.cpp
        INTERNAL True
        LIBRARY ceammc
        LINK ceammc_core)

    pd_add_external(NAME "ui.faust~"
        FILES lang_faust_ui_tilde_stub.cpp
        INTERNAL True
        LIBRARY ceammc
        LINK ceammc_core)
endif()

