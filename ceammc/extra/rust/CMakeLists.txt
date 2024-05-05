find_package(Rust)
if(Rust_FOUND)
    corrosion_import_crate(
        MANIFEST_PATH resvg/crates/c-api/Cargo.toml
        CRATE_TYPES staticlib
    )

    corrosion_import_crate(
        MANIFEST_PATH rimg/Cargo.toml
        CRATE_TYPES staticlib
    )

    if(WITH_ZEROCONF)
        if(WIN32)
            corrosion_import_crate(MANIFEST_PATH core/Cargo.toml CRATE_TYPES cdylib)
            install(FILES ${CMAKE_CURRENT_BINARY_DIR}/core_rust.dll DESTINATION bin)
        else()
            corrosion_import_crate(MANIFEST_PATH core/Cargo.toml CRATE_TYPES staticlib)
        endif()
    endif()

    if(WITH_GAMEPAD OR WITH_PRINTER)
        if(WIN32)
            corrosion_import_crate(MANIFEST_PATH hw/Cargo.toml CRATE_TYPES cdylib)
            install(FILES ${CMAKE_CURRENT_BINARY_DIR}/hw_rust.dll DESTINATION bin)
        else()
            corrosion_import_crate(MANIFEST_PATH hw/Cargo.toml CRATE_TYPES staticlib)
        endif()
    endif()

    if(WITH_MQTT OR WITH_HTTP OR WITH_FREESOUND OR WITH_TELEGRAM OR WITH_WEBSOCKET)
        if(WIN32)
            corrosion_import_crate(MANIFEST_PATH net/Cargo.toml CRATE_TYPES cdylib)
            install(FILES ${CMAKE_CURRENT_BINARY_DIR}/net_rust.dll DESTINATION bin)
        else()
            corrosion_import_crate(MANIFEST_PATH net/Cargo.toml CRATE_TYPES staticlib)
        endif()
    endif()

    if(WITH_SYSTEM_CMD)
        if(WIN32)
            corrosion_import_crate(MANIFEST_PATH system/Cargo.toml CRATE_TYPES cdylib)
            install(FILES ${CMAKE_CURRENT_BINARY_DIR}/system_rust.dll DESTINATION bin)
        else()
            corrosion_import_crate(MANIFEST_PATH system/Cargo.toml CRATE_TYPES staticlib)
        endif()
    endif()
endif()
