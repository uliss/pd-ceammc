add_library(ceammc_lang STATIC mod_lang.cpp)
target_include_directories(ceammc_lang
    PRIVATE "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>"
            ${PROJECT_BINARY_DIR})

set(FAUST_SEPARATE_EXTERNAL OFF)
if(APPLE)
    if(CMAKE_OSX_ARCHITECTURES STREQUAL "x86_64")
        set(FAUST_SEPARATE_EXTERNAL ON)
    endif()
endif()

if(WITH_FAUST)
    if(FAUST_SEPARATE_EXTERNAL)
        target_compile_definitions(ceammc_lang PRIVATE -DFAUST_SEPARATE_EXTERNAL)

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
        target_include_directories(ceammc_lang
            PRIVATE
                "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>"
                ${PROJECT_BINARY_DIR} # for config.h
                ${PROJECT_SOURCE_DIR}/ceammc/extra/faust
                ${PROJECT_SOURCE_DIR}/ceammc/extra/faust/faust/architecture)
        target_sources(ceammc_lang PRIVATE lang_faust_tilde.cpp lang_faust_ui_tilde.cpp)
        target_link_libraries(ceammc_lang PUBLIC ceammc_faust ceammc_nui)
    endif()
else()
    target_sources(ceammc_lang PRIVATE lang_faust_tilde.cpp lang_faust_ui_tilde_stub.cpp)
endif()

if(WITH_LUAJIT)
    pkg_search_module(LUAJIT luajit luajit-5.1 REQUIRED)
endif()

if(WITH_LUAJIT AND LUAJIT_FOUND)
    target_sources(ceammc_lang PRIVATE lang_luajit.cpp lua_cmd.cpp lua_func.cpp lua_interp.cpp)
    target_include_directories(ceammc_lang BEFORE PUBLIC ${LUAJIT_INCLUDE_DIRS})

    target_link_libraries(ceammc_lang PUBLIC ${LUAJIT_LINK_LIBRARIES} readerwriterqueue)
    target_compile_options(ceammc_lang PRIVATE ${LUAJIT_CFLAGS_OTHER})
    target_compile_definitions(ceammc_lang PRIVATE WITH_LUA)

    if(APPLE)
        target_link_options(ceammc_lang
            PRIVATE -pagezero_size 10000 -image_base 100000000)
    endif()
endif()

