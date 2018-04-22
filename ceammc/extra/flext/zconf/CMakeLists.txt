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
    
    pd_add_external(NAME zconf
        FILES ${ZCONF_SRC}
        INTERNAL True
        LIBRARY zconf
        LINK flext_static)

    target_compile_options(zconf PUBLIC -Wno-narrowing)
endif()
