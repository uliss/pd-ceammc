if(APPLE)
    set(CPACK_GENERATOR "ZIP;")
    set(CPACK_SOURCE_GENERATOR "TGZ;")
endif()

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "PureData")
set(CPACK_PACKAGE_VENDOR "http://ceammc.com")
set(CPACK_PACKAGE_CONTACT "http://ceammc.com")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.txt")
set(CPACK_PACKAGE_VERSION_MAJOR ${PD_MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${PD_MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH "${PD_BUGFIX_VERSION}_ceammc")

set(PD_DESKTOP_DIR "${CMAKE_INSTALL_PREFIX}/share/applications")
set(PD_MIME_DIR "${CMAKE_INSTALL_PREFIX}/share/mime/application")
set(PD_ICONS_DIR "${CMAKE_INSTALL_PREFIX}/share/icons")
set(PD_PIXMAPS_DIR "${CMAKE_INSTALL_PREFIX}/share/pixmaps")

find_program(XDG-MIME_EXECUTABLE xdg-mime)
find_program(XDG-DESKTOP-MENU_EXECUTABLE xdg-desktop-menu)
# Debian package
include(DpkgBuild)
if(DPKG_FOUND AND NOT WIN32)
    set(DESKTOP_FILE "puredata.desktop")
    # substitute version variables
    configure_file("${CMAKE_SOURCE_DIR}/ceammc/gui/linux/${DESKTOP_FILE}" ${CMAKE_CURRENT_BINARY_DIR})


    install(FILES "${CMAKE_SOURCE_DIR}/ceammc/gui/icons/puredata-ceammc.png"
        DESTINATION ${PD_PIXMAPS_DIR})
    install(FILES "${CMAKE_SOURCE_DIR}/ceammc/gui/icons/puredata-ceammc.png"
        DESTINATION ${PD_ICONS_DIR})
    install(FILES "${CMAKE_SOURCE_DIR}/ceammc/gui/linux/pd.xml"
        DESTINATION ${PD_MIME_DIR})
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${DESKTOP_FILE}"
        DESTINATION ${PD_DESKTOP_DIR})

    install(CODE "
        execute_process(COMMAND ${XDG-MIME_EXECUTABLE} install --novendor ${PD_MIME_DIR}/pd.xml)
        execute_process(COMMAND ${XDG-DESKTOP-MENU_EXECUTABLE} install --novendor ${PD_DESKTOP_DIR}/${DESKTOP_FILE})
        execute_process(COMMAND ${XDG-MIME_EXECUTABLE} default ${PD_DESKTOP_DIR}/${DESKTOP_FILE} application/x-pd-item)
    ")


    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libportaudio2, tk, tcl, tcllib, tklib")
    set(CPACK_DEBIAN_PACKAGE_VERSION "0.47.1")
    set(CPACK_SET_DESTDIR TRUE)
endif()

include(CPack)
