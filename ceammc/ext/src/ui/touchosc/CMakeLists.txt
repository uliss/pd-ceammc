add_library(touchosc STATIC
    layout.cpp control.cpp push_button.cpp tabpage.cpp xmlnode.cpp)

target_include_directories(touchosc
    PRIVATE
        "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>")

target_link_libraries(touchosc PRIVATE miniz)

