# this one is important
SET(CMAKE_SYSTEM_NAME Windows)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
SET(CMAKE_C_COMPILER   /usr/local/mingw/bin/i686-w64-mingw32-gcc)
SET(CMAKE_CXX_COMPILER /usr/local/mingw/bin/i686-w64-mingw32-g++)
SET(CMAKE_RC_COMPILER  /usr/local/mingw/bin/i686-w64-mingw32-windres)

# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH  /usr/local/mingw/i686-w64-mingw32)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)

set(WISH_PATH /usr/local/mingw/i686-w64-mingw32/bin/wish86.exe)
set(CMAKE_INSTALL_PREFIX /opt/mingw/pd)
set(WITH_DUMMY_AUDIO ON)
set(WINE_CROSSCOMPILE ON)
