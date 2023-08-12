if(WITH_VEROVIO)
    set(BUILD_AS_LIBRARY ON)
    add_subdirectory(verovio/cmake)
    target_compile_features(verovio PRIVATE cxx_std_17)
    target_include_directories(verovio PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    if(WIN32)
        target_include_directories(verovio PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/verovio/include/win32)
    endif()

    target_include_directories(verovio INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
    install(DIRECTORY verovio/data
        DESTINATION ${PD_DOC_INSTALL_PATH}/extra/ceammc/music/verovio
        FILES_MATCHING PATTERN "*.xml" PATTERN "*.svg" PATTERN "*.css")

    if(WIN32)
        install(TARGETS verovio RUNTIME DESTINATION extra/ceammc)
    endif()
endif()
