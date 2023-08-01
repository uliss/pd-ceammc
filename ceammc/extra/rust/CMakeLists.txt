set(WITH_RESVG ON)

if(WITH_RESVG)
corrosion_import_crate(
    MANIFEST_PATH resvg/crates/c-api/Cargo.toml
    CRATE_TYPES staticlib
)
endif()
