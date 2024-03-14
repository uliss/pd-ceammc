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

    corrosion_import_crate(
        MANIFEST_PATH core/Cargo.toml
        CRATE_TYPES staticlib
    )

    corrosion_import_crate(
        MANIFEST_PATH hw/Cargo.toml
        CRATE_TYPES staticlib
    )
endif()
