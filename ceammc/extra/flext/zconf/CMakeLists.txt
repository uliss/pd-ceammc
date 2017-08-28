find_package(DNSSD)

if(UNIX OR MACOSX)
    add_definitions(-Wno-c++11-narrowing)
endif()

if(DNSSD_FOUND) 
    set(ZCONF_SRC
        zconf/zconf.cpp
        zconf/zconf_browse.cpp
        zconf/zconf_domains.cpp
        zconf/zconf_meta.cpp
        zconf/zconf_resolve.cpp
        zconf/zconf_service.cpp
    )
    
    pd_add_extension(NAME zconf
        FILES ${ZCONF_SRC}
        INTERNAL True
        LIBRARY zconf
        LINK flext_static)
endif()
