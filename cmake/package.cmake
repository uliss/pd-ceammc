if(APPLE)
    set(CPACK_GENERATOR "ZIP;")
    set(CPACK_SOURCE_GENERATOR "TGZ;")
endif()

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "realtime graphical computer music system")
set(CPACK_PACKAGE_DESCRIPTION "PureData distribution used in CEAM (Center of Electroacoustic Music, Moscow Conservatory, Russia")
set(CPACK_PACKAGE_VENDOR "http://ceammc.com")
set(CPACK_PACKAGE_CONTACT "http://ceammc.com")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.txt")
set(CPACK_PACKAGE_VERSION_MAJOR ${CEAMMC_DISTRIB_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR "${PD_MAJOR_VERSION}.${PD_MINOR_VERSION}")
set(CPACK_PACKAGE_VERSION_PATCH ${PD_BUGFIX_VERSION})
set(CPACK_STRIP_FILES ON)

set(PD_DESKTOP_DIR "${CMAKE_INSTALL_PREFIX}/share/applications")
set(PD_MIME_DIR "${CMAKE_INSTALL_PREFIX}/share/mime/packages")
set(PD_ICONS_DIR "${CMAKE_INSTALL_PREFIX}/share/icons")
set(PD_PIXMAPS_DIR "${CMAKE_INSTALL_PREFIX}/share/pixmaps")

find_program(XDG-MIME_EXECUTABLE xdg-mime)
find_program(XDG-DESKTOP-MENU_EXECUTABLE xdg-desktop-menu)

# Debian package
include(DpkgBuild)
if(DPKG_FOUND AND NOT WIN32)
    set(DESKTOP_FILE "pd-ceammc.desktop")
    set(MIME_FILE    "pd-ceammc.xml")
    # substitute version variables
    configure_file("${CMAKE_SOURCE_DIR}/ceammc/gui/linux/${DESKTOP_FILE}" ${CMAKE_CURRENT_BINARY_DIR})

    install(FILES "${CMAKE_SOURCE_DIR}/ceammc/gui/icons/puredata-ceammc.png"
        DESTINATION ${PD_PIXMAPS_DIR})
    install(FILES "${CMAKE_SOURCE_DIR}/ceammc/gui/icons/puredata-ceammc.png"
        DESTINATION ${PD_ICONS_DIR})
    install(FILES "${CMAKE_SOURCE_DIR}/ceammc/gui/icons/puredata-ceammc.png"
        DESTINATION ${PD_ICONS_DIR}/hicolor/512x512/apps)
    install(FILES "${CMAKE_SOURCE_DIR}/ceammc/gui/icons/pd_ceammc5.svg"
        DESTINATION ${PD_ICONS_DIR}/hicolor/scalable/apps/puredata-ceammc.svg)
    # install to tcl dir because pd-gui.tcl sets app icon manually
    install(FILES "${CMAKE_SOURCE_DIR}/ceammc/gui/icons/puredata-ceammc.png"
        DESTINATION ${PD_GUI_PLUGINS_PATH})
    # MIME file for .pd documents
    install(FILES "${CMAKE_SOURCE_DIR}/ceammc/gui/linux/${MIME_FILE}" DESTINATION ${PD_MIME_DIR})
    # Desktop files
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${DESKTOP_FILE}" DESTINATION ${PD_DESKTOP_DIR})

    set(CPACK_GENERATOR "DEB")
    set(CPACK_PACKAGE_VERSION "${CEAMMC_DISTRIB_VERSION}-${PD_TEXT_VERSION_SHORT}")
    set(CPACK_PACKAGE_FILE_NAME "pd-ceammc-${CPACK_PACKAGE_VERSION}_${LSB_CODENAME}_${LSB_PROCESSOR_ARCH}")
    set(CPACK_DEBIAN_PACKAGE_NAME "pd-ceammc")
    set(CPACK_DEBIAN_PACKAGE_VERSION ${CEAMMC_DISTRIB_VERSION})
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "tk, tcl, tcllib, tklib, xdg-utils")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Serge Poltavski <serge.poltavski@gmail.com>")
    set(CPACK_DEBIAN_PACKAGE_SECTION "sound")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/uliss/pure-data")
    set(CPACK_DEBIAN_PACKAGE_PROVIDES "pd")
    # note: linebreak and single space indent!
    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Visual audio programming language
  ${CPACK_PACKAGE_DESCRIPTION}")
#    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
    set(CPACK_SET_DESTDIR TRUE)
    set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS ON)

    set(POSTINST_FILE "${CMAKE_SOURCE_DIR}/ceammc/distrib/linux/deb/postinst")
    set(POSTRM_FILE "${CMAKE_SOURCE_DIR}/ceammc/distrib/linux/deb/postrm")
  
    set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${POSTINST_FILE};${POSTRM_FILE}")

#    install(FILES "${PROJECT_SOURCE_DIR}/ceammc/distrib/gpl-3.0.txt"
#        DESTINATION "share/doc/${CPACK_DEBIAN_PACKAGE_NAME}/copyright"
#        PERMISSIONS
#        OWNER_WRITE OWNER_READ
#        GROUP_READ
#        WORLD_READ
#    )

    # debian changelog
    include(GNUInstallDirs)

    set(CHANGELOG "${PROJECT_SOURCE_DIR}/ceammc/CHANGELOG.md")
    add_custom_command(
        OUTPUT "${PROJECT_BINARY_DIR}/changelog.gz"
        COMMAND gzip -cn9 "${CHANGELOG}" > "${PROJECT_BINARY_DIR}/changelog.gz"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        DEPENDS "${CHANGELOG}"
        COMMENT "Compressing changelog"
    )
    add_custom_target(changelog ALL DEPENDS "${PROJECT_BINARY_DIR}/changelog.gz")

    install(FILES "${PROJECT_BINARY_DIR}/changelog.gz"
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/doc/${CPACK_DEBIAN_PACKAGE_NAME}"
    )
endif()

include(CPack)

add_custom_target(src-zip
    COMMAND "${CMAKE_SOURCE_DIR}/ceammc/distrib/git-archive-all/git_archive_all.py"
            "pd-ceammc-${CEAMMC_DISTRIB_VERSION}-src.zip")

add_custom_target(src-tar
    COMMAND "${CMAKE_SOURCE_DIR}/ceammc/distrib/git-archive-all/git_archive_all.py"
            "pd-ceammc-${CEAMMC_DISTRIB_VERSION}-src.tar.gz")
