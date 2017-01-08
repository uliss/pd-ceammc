find_program(PD_DOC2PD pd_doc2pd)
find_program(PD_MAKELIB pd_makelibrary)
find_program(PD_LIB2PD pd_lib2pd)
find_program(PD_CAT2PD pd_cat2pd)

set(DOC_FILES
    data.fifo
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
    flt.dcblock~
    flt.dcblock2~
    flt.highshelf~
    flt.lowshelf~
    flt.eq_peak~
    fx.pitchshift~
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
    list.gen
    list.last
    list.length
    list.max
    list.min
    list.normalize
    list.product
    list.range
    list.reduce
    list.resize
    list.reverse
    list.shuffle
    list.sort
    list.sum
    list.walk
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
    osc.impulse~
    osc.pulse~
    osc.saw~
    osc.sinfb~
    osc.square~
    osc.tri~
    pass.changed
    pass.if
    system.memsize
    system.memused
    vector.dot
    vector.length
    )

if(NOT PD_XML2PD)
    message(STATUS "pd_xml2pd not found.")
else()
    message(STATUS "pd_xml2pd found at: ${PD_XML2PD}")
    set(DOC_PD_FILES)
    set(DOC_XLET_DB_FILES)
    set(DOC_PDDOC_FILES)
    foreach(f ${DOC_FILES})
        set(fname "${CMAKE_CURRENT_SOURCE_DIR}/${f}-help.pd")
        set(fname_xlet "${CMAKE_CURRENT_SOURCE_DIR}/${f}-xlet_db.txt")
        set(fname_pddoc "${CMAKE_CURRENT_SOURCE_DIR}/${f}.pddoc")
        list(APPEND DOC_PD_FILES ${fname})
        list(APPEND DOC_XLET_DB_FILES ${fname_xlet})
        list(APPEND DOC_PDDOC_FILES ${fname_pddoc})
        add_custom_command(
            OUTPUT ${fname}
            DEPENDS "${f}.pddoc"
            COMMAND ${PD_DOC2PD}
                 --force
                 --version "${CEAMMC_LIB_VERSION}"
                 --website "${CEAMMC_LIB_HOME}"
                 --xlet-db "ceammc.db"
                "${f}.pddoc" ${fname}
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
    endforeach()

    add_custom_command(
        OUTPUT "ceammc.db"
        DEPENDS ${DOC_XLET_DB_FILES}
        COMMAND cat "*-xlet_db.txt" | sort > ceammc.db
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

    add_custom_command(
        OUTPUT "ceammc_lib.xml"
        DEPENDS ${DOC_PDDOC_FILES}
        COMMAND ${PD_MAKELIB}
            --library ceammc
            --version "${CEAMMC_LIB_VERSION}"
            --output "ceammc_lib.xml" ${DOC_PDDOC_FILES}
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

    add_custom_command(
        OUTPUT "ceammc-help.pd"
        DEPENDS "ceammc_lib.xml"
        COMMAND ${PD_LIB2PD} "ceammc_lib.xml"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

    add_custom_target(ceammc_pddoc
        DEPENDS ${DOC_PD_FILES} "ceammc.db" ceammc_pddoc_cat "ceammc_lib.xml" "ceammc-help.pd"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

    add_custom_target(ceammc_pddoc_cat
        DEPENDS ${DOC_PD_FILES} "ceammc_lib.xml"
        COMMAND pd_cat2pd "ceammc_lib.xml"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif()

