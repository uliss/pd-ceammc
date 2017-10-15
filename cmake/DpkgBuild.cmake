#
# DPKG Finder
# Authors: Rohit Yadav <rohityadav89@gmail.com>
#

find_program(DPKG
    NAMES dpkg-deb
    PATHS "/usr/bin") #Add paths here

if(DPKG)
    get_filename_component(DPKG_PATH ${DPKG} ABSOLUTE)
    message(STATUS "Found dpkg-deb : ${DPKG_PATH}")
    set(DPKG_FOUND "YES")
else ()
    message(STATUS "dpkg-deb NOT found. deb generation will not be available")
    set(DPKG_FOUND "NO")
endif()
