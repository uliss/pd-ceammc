add_library(qrcode
    STATIC
        qrcodegen.cpp
        qrcodegen.hpp
)

if(UNIX)
    set_target_properties(qrcode PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
