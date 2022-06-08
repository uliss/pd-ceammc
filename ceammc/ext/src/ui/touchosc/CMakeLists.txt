add_library(touchosc STATIC
    layout.cpp control.cpp tabpage.cpp xmlnode.cpp)

target_include_directories(touchosc
    PRIVATE
        "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>")

