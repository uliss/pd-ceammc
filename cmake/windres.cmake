if(MSYS OR WIN32)
#    find_program(WINDRES windres)
    
#    if(NOT WINDRES)
#        message(FATAL_ERROR "windres not found")
#    endif()

#    set(CMAKE_RC_COMPILER_INIT ${WINDRES})
    enable_language(RC)
    set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
endif()
