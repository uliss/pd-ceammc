find_package(DNSSD)

if(DNSSD_FOUND) 
    set(ZCONF_SRC
        zconf.cpp
        zconf_browse.cpp
        zconf_domains.cpp
        zconf_meta.cpp
        zconf_resolve.cpp
        zconf_service.cpp
    )

    if(APPLE)
        if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
#            add_definitions(-D)
        endif()
    endif()
    
    pd_add_extension(NAME zconf
        FILES ${ZCONF_SRC}
        INTERNAL True
        LIBRARY zconf
        LINK flext_static)
endif()
