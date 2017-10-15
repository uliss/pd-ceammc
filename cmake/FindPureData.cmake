# - Find PureData
# Find the PureData includes
#
#  PUREDATA_INCLUDE_DIR - where to find m_pd.h, etc.
#  PUREDATA_FOUND - True if PureData found.

if(PUREDATA_INCLUDE_DIR)
    # Already in cache, be silent
    set(PUREDATA_FIND_QUIETLY TRUE)
endif()

find_path(PUREDATA_INCLUDE_DIR
    NAMES
        m_pd.h
    PATHS
        ${CMAKE_SOURCE_DIR}/src
        ${PD_INCLUDE_DIR}
        /usr/include
        /usr/local/include
        /opt/local/include
        /Applications/Pd.app/Contents/Resources/include
        /Applications/Pd.app/Contents/Resources/src
        $ENV{PROGRAMFILES}/Pd/include
    PATH_SUFFIXES pd pd_ceammc
)

# handle the QUIETLY and REQUIRED arguments and set PUREDATA_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PureData DEFAULT_MSG PUREDATA_INCLUDE_DIR)

mark_as_advanced(PUREDATA_INCLUDE_DIR)



