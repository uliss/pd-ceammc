add_library(qrcode
    STATIC
        qrcodegen.cpp
        qrcodegen.hpp
)

target_include_directories(qrcode PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(UNIX)
    set_target_properties(qrcode PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
