set(PD_EXTERNAL_EXTENSION)
set(PD_EXTERNAL_CFLAGS)
set(PD_EXTERNAL_LDFLAGS)

if(APPLE)
    set(PD_EXTERNAL_EXTENSION d_fat)
    set(PD_EXTERNAL_CFLAGS "-fPIC")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O3 -funroll-loops -fomit-frame-pointer")
    set(PD_EXTERNAL_LDFLAGS "-bundle -undefined dynamic_lookup")
endif()
