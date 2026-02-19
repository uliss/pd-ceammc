find_package(Rust)
if(Rust_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ceammc_config.rs
                 ${CMAKE_CURRENT_SOURCE_DIR}/apps/src/ceammc_config.rs @ONLY)

  corrosion_import_crate(MANIFEST_PATH resvg/crates/c-api/Cargo.toml
                         CRATE_TYPES staticlib)

  corrosion_import_crate(MANIFEST_PATH rimg/Cargo.toml CRATE_TYPES staticlib)

  if(WITH_RUST_APPS)
    corrosion_import_crate(MANIFEST_PATH apps/Cargo.toml)

    install(
      FILES $<TARGET_FILE:rhvoice_download>
      DESTINATION ${CEAMMC_INSTALL_PATH}
      PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ
                  WORLD_EXECUTE)

    corrosion_import_crate(MANIFEST_PATH apps/util/Cargo.toml)

    # ceam-util executable
    if(UNIX AND NOT APPLE)
      cmake_path(SET ceam_util_install_dir "bin")
    else()
      cmake_path(SET ceam_util_install_dir ${CEAMMC_INSTALL_PATH})
    endif()

    install(
      FILES $<TARGET_FILE:ceam-util>
      DESTINATION ${ceam_util_install_dir}
      PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ
                  WORLD_EXECUTE)

    set(RUST_DYLIBFIX
        ${CMAKE_CURRENT_BINARY_DIR}/dylibfix
        CACHE STRING FORCE "set dylibfix app")
  endif()

  if(WIN32)
    corrosion_import_crate(MANIFEST_PATH shared/Cargo.toml CRATE_TYPES cdylib)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ceammc_shared_rs.dll
            DESTINATION bin)
  else()
    corrosion_import_crate(MANIFEST_PATH shared/Cargo.toml CRATE_TYPES
                           staticlib)
  endif()
  # include cbindgen generated headers
  target_include_directories(
    ceammc_shared_rs
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/shared>)

  if(WITH_RUST_CORE)
    if(WIN32)
      corrosion_import_crate(MANIFEST_PATH core/Cargo.toml CRATE_TYPES cdylib)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/core_rust.dll DESTINATION bin)
    else()
      corrosion_import_crate(MANIFEST_PATH core/Cargo.toml CRATE_TYPES
                             staticlib)
    endif()
    target_include_directories(
      core_rust INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/core>)
  endif()

  if(WITH_RUST_HW)
    if(WIN32)
      corrosion_import_crate(MANIFEST_PATH hw/Cargo.toml CRATE_TYPES cdylib)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/hw_rust.dll DESTINATION bin)
    else()
      set(_features "")
      if(WITH_PRINTER)
        set(_features "cups")
      endif()
      corrosion_import_crate(MANIFEST_PATH hw/Cargo.toml CRATE_TYPES staticlib
                             FEATURES ${_features})
    endif()
  endif()

  if(WITH_RUST_PROTO)
    if(WIN32)
      corrosion_import_crate(MANIFEST_PATH proto/Cargo.toml CRATE_TYPES cdylib)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/proto_rust.dll DESTINATION bin)
    else()
      corrosion_import_crate(MANIFEST_PATH proto/Cargo.toml CRATE_TYPES
                             staticlib)
    endif()
  endif()

  if(WITH_RUST_NET)
    set(RUST_NET_CRATE_TYPE staticlib)
    if(APPLE OR WIN32)
      set(RUST_NET_CRATE_TYPE cdylib)
    endif()

    corrosion_import_crate(MANIFEST_PATH net/Cargo.toml CRATE_TYPES
                           ${RUST_NET_CRATE_TYPE})

    if(WIN32)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/net_rust.dll DESTINATION bin)
    endif()
  endif()

  if(WITH_RUST_SYSTEM)
    if(WIN32)
      corrosion_import_crate(MANIFEST_PATH system/Cargo.toml CRATE_TYPES cdylib)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/system_rust.dll DESTINATION bin)
    else()
      corrosion_import_crate(MANIFEST_PATH system/Cargo.toml CRATE_TYPES
                             staticlib)
    endif()
  endif()

  if(WITH_RUST_SYNTH)
    if(WIN32)
      corrosion_import_crate(MANIFEST_PATH synth/Cargo.toml CRATE_TYPES cdylib)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/synth_rust.dll DESTINATION bin)
    else()
      corrosion_import_crate(MANIFEST_PATH synth/Cargo.toml CRATE_TYPES
                             staticlib)
    endif()
  endif()
endif()

file(GLOB_RECURSE RUST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR} *.rs *.toml)
add_custom_target(rust_src SOURCES ${RUST_SOURCES})
