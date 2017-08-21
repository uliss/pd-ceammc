set(LEAPMOTION_SRC
    main.cpp
    Dispatcher.cpp
    Dispatcher.h
)

if(LEAPMOTION_FOUND)
    include_directories(${LEAPMOTION_INCLUDES})
   
    pd_add_extension(NAME "leapmotion"
        FILES ${LEAPMOTION_SRC}
        INTERNAL True
        LIBRARY leapmotion
        LINK flext_static ${LEAPMOTION_LIBRARIES})
else()
    message(STATUS "LeapMotion SDK is not found")
    message(STATUS "   specify SDK path with cmake -DLEAPMOTION_ROOT='..' configure option")
endif()
