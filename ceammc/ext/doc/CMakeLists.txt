find_program(PD_DOC2PD pd_doc2pd)
find_program(PD_MAKELIB pd_makelibrary)
find_program(PD_LIB2PD pd_lib2pd)
find_program(PD_CAT2PD pd_cat2pd)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/about.pd ${CMAKE_CURRENT_BINARY_DIR}/about.pd @ONLY)

set(DOC_FILES
    data.fifo
    data.list
    dyn.comp~
    dyn.comp2~
    dyn.gate~
    dyn.gate2~
    dyn.limit~
    dyn.limit2~
    dyn.softclip~
    env.ar~
    env.adsr~
    env.follow~
    flt.c_bpf~
    flt.c_hpf~
    flt.c_lpf~
    flt.dcblock~
    flt.dcblock2~
    flt.biquad~
    flt.highshelf~
    flt.lowshelf~
    flt.notch~
    flt.resonbp~
    flt.eq_peak~
    fx.freqshift~
    fx.pitchshift~
    fx.freeverb~
    global.float
    global.int
    global.list
    is_any
    is_bang
    is_float
    is_odd
    is_pointer
    is_symbol
    is_list
    is_even
    live.capture~
    list.apply_to
    list.at
    list.choice
    list.count
    list.count_if
    list.deinterleave
    list.delta
    list.each
    list.equal
    list.first
    list.interleave
    list.integrator
    list.gen
    list.last
    list.length
    list.max
    list.mean
    list.min
    list.normalize
    list.product
    list.range
    list.reduce
    list.resize
    list.reverse
    list.seq
    list.shuffle
    list.sort
    list.sum
    list.unpack
    list.walk
    list.wrap
    list.unwrap
    lfo.impulse~
    lfo.square~
    lfo.tri~
    math.abs
    math.acos
    math.acosh
    math.asin
    math.asinh
    math.atan
    math.atanh
    math.cbrt
    math.ceil
    math.cos
    math.cosh
    math.e
    math.exp
    math.exp2
    math.floor
    math.inf
    math.log
    math.log10
    math.log2
    math.nan
    math.pi
    math.round
    math.sin
    math.sinh
    math.sqrt
    math.tan
    math.tanh
    math.trunc
    msg
    noise.pink~
    noise.white~
    osc.impulse~
    osc.pulse~
    osc.saw~
    osc.sinfb~
    osc.square~
    osc.tri~
    pass.changed
    pass.if
    prop.get
    prop.set
    replace
    snd.file
    symbol.length
    system.memsize
    system.memused
    system.screen_size
    ui.bang
    ui.bpfunc
    ui.display
    ui.knob
    ui.matrix
    ui.scope~
    ui.spectroscope~
    ui.toggle
    vector.distance
    vector.dot
    vector.length
    vector.normalize
    )

if(NOT PD_LIB2PD)
    message(STATUS "pd_lib2pd not found.")
else()
    message(STATUS "pd_lib2pd found at: ${PD_LIB2PD}")
    set(DOC_PD_FILES)
    set(DOC_XLET_DB_FILES)
    set(DOC_PDDOC_FILES)
    foreach(f ${DOC_FILES})
        set(fname "${CMAKE_CURRENT_BINARY_DIR}/${f}-help.pd")
        set(fname_pddoc "${CMAKE_CURRENT_SOURCE_DIR}/${f}.pddoc")
        list(APPEND DOC_PD_FILES ${fname})
        list(APPEND DOC_PDDOC_FILES ${fname_pddoc})
        add_custom_command(
            OUTPUT "${fname}"
            DEPENDS "${fname_pddoc}"
            COMMAND ${PD_DOC2PD}
                 --force
                 --version "${CEAMMC_LIB_VERSION}"
                 --website "${CEAMMC_LIB_HOME}"
                 --xlet-db "${CMAKE_CURRENT_SOURCE_DIR}/ceammc.db"
                "${fname_pddoc}" ${fname}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${fname} ${CMAKE_CURRENT_SOURCE_DIR}
            VERBATIM)
    endforeach()

    add_custom_command(
        OUTPUT "ceammc_lib.xml"
        DEPENDS ${DOC_PDDOC_FILES}
        COMMAND ${PD_MAKELIB}
            --library ceammc
            --version "${CEAMMC_LIB_VERSION}"
            --output ceammc_lib.xml ${DOC_PDDOC_FILES}
            --search-path ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ceammc_lib.xml ${CMAKE_CURRENT_SOURCE_DIR})

    add_custom_command(
        OUTPUT "ceammc-help.pd"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/ceammc_lib.xml"
        COMMAND ${PD_LIB2PD} "${CMAKE_CURRENT_SOURCE_DIR}/ceammc_lib.xml"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "ceammc-help.pd" ${CMAKE_CURRENT_SOURCE_DIR})

    add_custom_target(ceammc_pddoc_cat
        DEPENDS ${DOC_PD_FILES} "ceammc_lib.xml"
        COMMAND ${PD_CAT2PD} "ceammc_lib.xml"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

    add_custom_target(ceammc_pddoc
        DEPENDS ${DOC_PD_FILES} "ceammc_lib.xml" ceammc_pddoc_cat "ceammc-help.pd"
        COMMAND cat "*-xlet_db.txt" | sort > ceammc.db
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ceammc.db ${CMAKE_CURRENT_SOURCE_DIR})
endif()

if(WIN32)
    set(DOC_INSTALL_PATH "extra/ceammc")
else()
    set(DOC_INSTALL_PATH "${PD_DOC_INSTALL_PATH}/extra/ceammc")
endif()

    foreach(_doc_file ${DOC_FILES})
        install(FILES "${_doc_file}-help.pd"
            DESTINATION ${DOC_INSTALL_PATH}
            PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
    endforeach()

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/about.pd" DESTINATION ${DOC_INSTALL_PATH}
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

    install(FILES "index-help.pd" DESTINATION "${DOC_INSTALL_PATH}/.."
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

    install(
        CODE "file( GLOB _PD_CATEGORY_HELP_FILES \"${CMAKE_CURRENT_SOURCE_DIR}/ceammc*-help.pd\" )"
        CODE "file( INSTALL \${_PD_CATEGORY_HELP_FILES}
            DESTINATION \"${CMAKE_INSTALL_PREFIX}/${DOC_INSTALL_PATH}\")"
    )


file(GLOB _PDDOC_FILES "*.pddoc")
file(GLOB _PDDOC_XML_FILES "*.xml")
add_custom_target(pddoc_files SOURCES ${_PDDOC_FILES} ${_PDDOC_XML_FILES})

