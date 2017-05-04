set(CEAMMC_LIB_VERSION "0.2")
set(CEAMMC_LIB_HOME "https://github.com/uliss/pure-data")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ceammc_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/ceammc_config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(extra)
add_subdirectory(gui)
add_subdirectory(ext)
add_subdirectory(distrib)
add_subdirectory(faust)
add_subdirectory(py)

set(POT_TEMPLATE "${CMAKE_SOURCE_DIR}/po/template.pot")
find_program(XGETTEXT xgettext)

if(XGETTEXT)
    file(GLOB _CICM_FILES RELATIVE ${CMAKE_SOURCE_DIR}/po "${CMAKE_CURRENT_SOURCE_DIR}/extra/CicmWrapper/Sources/*.c")
    file(GLOB _CREAM_FILES RELATIVE ${CMAKE_SOURCE_DIR}/po "${CMAKE_CURRENT_SOURCE_DIR}/extra/CreamLibrary/Sources/*.cpp")
    file(GLOB _CEAMMC_FILES RELATIVE ${CMAKE_SOURCE_DIR}/po "${CMAKE_CURRENT_SOURCE_DIR}/ext/src/*.cpp")

    add_custom_target(ceammc_messages
        COMMAND ${XGETTEXT} --join-existing --from-code=UTF-8
            --language=C --keyword=_
            --sort-by-file --output=${POT_TEMPLATE}
            --package-name="Pure Data" --package-version=${PD_TEXT_VERSION_FULL}
            --copyright-holder='This file is put in the public domain'
            --foreign-user --msgid-bugs-address=pd-dev@iem.at
            ${_CICM_FILES} ${_CREAM_FILES} ${_CEAMMC_FILES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()


