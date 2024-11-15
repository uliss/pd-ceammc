#!/bin/bash

OUT_FILE="@CMAKE_CURRENT_BINARY_DIR@/ceammc_popup_menu_extra.tcl"
WITH_FLEXT_VASP="@WITH_FLEXT_VASP@"
WITH_EXT_AUTOTUNE="@WITH_EXT_AUTOTUNE@"
WITH_EXT_FLEXT="@WITH_EXT_FLEXT@"
LEAPMOTION_FOUND="@LEAPMOTION_FOUND@"


function is_set() {
    echo $1 | grep -ie "^on$" -e "^true$" > /dev/null
}

echo "{extra" > $OUT_FILE

# autotune flext external
is_set ${WITH_EXT_AUTOTUNE}
if [ "$?" -eq 0 ]
then
cat << EOF >> $OUT_FILE
    {autotune~
        {autotune~}
    }
EOF
fi

# disis_munger flext external
is_set ${WITH_EXT_FLEXT}
if [ "$?" -eq 0 ]
then
cat << EOF >> $OUT_FILE
    {disis
        {disis_munger~}
    }
EOF
fi

# leapmotion flext external
is_set ${LEAPMOTION_FOUND}
if [ "$?" -eq 0 ]
then
cat << EOF >> $OUT_FILE
    {leapmotion
        {leapmotion}
    }
EOF
fi

# soundtouch~
cat << EOF >> $OUT_FILE
    {soundtouch~
        {soundtouch~}
    }
EOF

# vasp flext external
is_set ${WITH_FLEXT_VASP}
if [ "$?" -eq 0 ]
then
cat << EOF >> $OUT_FILE
    {vasp_library
        {vasp vasp.abs vasp.cabs vasp.ccopy vasp.cfft vasp.channel
        vasp.channel? vasp.channels? vasp.check vasp.cnoise vasp.cnorm
        vasp.copy vasp.cosc vasp.cpowi vasp.cset vasp.csqr vasp.dif
        vasp.exp vasp.fhp vasp.fix vasp.flp vasp.frames vasp.frames*
        vasp.frames+ vasp.frames/ vasp.frames? vasp.gate vasp.gather
        vasp.imm vasp.int vasp.join vasp.list vasp.log vasp.m vasp.max
        vasp.min vasp.minmax vasp.mirr vasp.noise vasp.nonzero vasp.offset
        vasp.offset+ vasp.offset? vasp.osc vasp.part vasp.peaks vasp.phasor
        vasp.polar vasp.pow vasp.radio vasp.rect vasp.rfft vasp.rgate
        vasp.rmax vasp.rmin vasp.rot vasp.rpeaks vasp.rpow vasp.rvalleys
        vasp.set vasp.shift vasp.sign vasp.size vasp.size* vasp.size+
        vasp.size/ vasp.size? vasp.soffset vasp.spit vasp.split vasp.sqr
        vasp.sqrt vasp.ssqr vasp.ssqrt vasp.sync vasp.tilt vasp.update vasp.valleys
        vasp.vector vasp.vectors? vasp.window vasp.xmirr vasp.xrot vasp.xshift vasp.xtilt
        vasp.xwindow}
    }
EOF
fi

echo "}" >> $OUT_FILE

exit 0
