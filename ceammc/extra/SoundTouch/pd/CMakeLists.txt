include(PdExtension)
# for m_pd.h and others
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/ceammc/ext/src/lib)

pd_add_extension(NAME soundtouch~
    FILES soundtouch_tilde.cpp
    INTERNAL False
    LIBRARY ceammc
    LINK ceammc_core soundtouch) 

file(GLOB _PDDOC_FILES "*.pddoc")
file(GLOB _PDDOC_XML_FILES "*.xml")

set(DOC_FILES soundtouch~)

if(PD_LIB2PD)
    make_pddoc_lib(
        NAME soundtouch
        VERSION 0.1
        HOME "https://www.surina.net/soundtouch/"
        FILES ${DOC_FILES})
endif()

add_custom_target(sountouch_pddoc_files SOURCES ${_PDDOC_FILES} ${_PDDOC_XML_FILES})

