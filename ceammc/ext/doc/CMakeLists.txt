find_program(PD_DOC2PD pd_doc2pd)
find_program(PD_MAKELIB pd_makelibrary)
find_program(PD_LIB2PD pd_lib2pd)
find_program(PD_CAT2PD pd_cat2pd)
find_program(TIDY tidy)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/about.pd ${CMAKE_CURRENT_BINARY_DIR}/about.pd @ONLY)

set(DOC_FILES
    conv.bpm2hz
    conv.bpm2ms
    conv.bpm2sec
    conv.lin2curve
    conv.lin2lin
    conv.lin2exp
    conv.sec2str
    conv.str2sec
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
    expand_env
    flow.sync
    flt.bpf12~
    flt.bpf24~
    flt.c_bpf~
    flt.c_highshelf~
    flt.c_hpf~
    flt.c_lowshelf~
    flt.c_lpf~
    flt.c_notch~
    flt.c_peak~
    flt.dcblock~
    flt.dcblock2~
    flt.biquad~
    flt.highshelf~
    flt.hpf12~
    flt.hpf24~
    flt.lowshelf~
    flt.lpf12~
    flt.lpf24~
    flt.notch~
    flt.resonbp~
    flt.eq_peak~
    fx.flanger~
    fx.freqshift~
    fx.distortion~
    fx.pitchshift~
    fx.freeverb~
    fx.freeverb2~
    fx.zita_rev1~
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
    list.all_of
    list.any_of
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
    list.none_of
    list.normalize
    list.product
    list.range
    list.reduce
    list.remove_if
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
    math.and
    math.approx
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
    math.neg
    math.pi
    math.reciprocal
    math.round
    math.sign
    math.sin
    math.sinh
    math.sqrt
    math.squared
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
    pass.this
    path.basename
    path.dirname
    prop.get
    prop.set
    random.int
    reject.if
    reject.this
    replace
    snd.file
    symbol.equal
    symbol.length
    system.getenv
    system.hostname
    system.memsize
    system.memused
    system.screen_size
    ui.bang
    ui.bpfunc
    ui.display
    ui.keyboard
    ui.knob
    ui.matrix
    ui.scope~
    ui.slider2d
    ui.sliders
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

if(TIDY)
    message(STATUS "HTMLtidy found: ${TIDY}. The 'ceammc_pddoc_format' target available.")
    add_custom_target(ceammc_pddoc_format
        COMMAND ls *.pddoc | while read f\;
        do
            ${TIDY} -quiet -xml --indent auto
            --indent-spaces 4 --wrap 100 --wrap-sections no $$f > "${CMAKE_CURRENT_BINARY_DIR}/$$f" \;
            ${CMAKE_COMMAND} -E copy_if_different
                "${CMAKE_CURRENT_BINARY_DIR}/$$f" . \;
            echo "Format pddoc file: $$f" \;
        done
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif()


file(GLOB _PDDOC_FILES "*.pddoc")
file(GLOB _PDDOC_XML_FILES "*.xml")
add_custom_target(pddoc_files SOURCES ${_PDDOC_FILES} ${_PDDOC_XML_FILES} ceammc.db)

