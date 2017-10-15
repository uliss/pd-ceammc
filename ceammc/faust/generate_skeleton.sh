#!/bin/bash

FAUST=faust
FAUST_OPT="-vec -vs 64 -i"
ERROR="\033[31mError:\033[0m"

usage() {
    echo "Usage: $(basename $0) PREFIX NAME"
}

if [ $# -ne 2 ]
then
    usage
    exit 1
fi

PREFIX=$1
NAME=$2
TEMPLATE="simple_pd_control_ext.cpp"
H_FILE="../ext/src/${PREFIX}_${NAME}.h"
CPP_FILE="../ext/src/${PREFIX}_${NAME}.cpp"
DSP_FILE="${PREFIX}_${NAME}.dsp"

if [ ! -f ${DSP_FILE} ]
then
    echo "Faust DSP file not found: ${DSP_FILE}"
    exit 1
fi

if [ ! -f ${TEMPLATE} ]
then
    echo "Puredata CPP template not found: \"${TEMPLATE}\""
    exit 1
fi

if [ -f ${H_FILE} ]
then
    echo "${ERROR} C++ header file \"${H_FILE}\" already exists. exitting..."
    echo "       Remove it manually to overwrite."
    exit 1
fi

if [ -f ${CPP_FILE} ]
then
    echo "${ERROR} C++ source file \"${CPP_FILE}\" already exists. exitting..."
    echo "       Remove it manually to overwrite."
    exit 1
fi

echo "Generating C++ header file \"${H_FILE}\"..."

${FAUST} ${FAUST_OPT} -i -a "${TEMPLATE}" -cn ${NAME} "${DSP_FILE}" -o "${H_FILE}"

echo "Generating C++ source file \"${CPP_FILE}\"..."

cat << EOF > "${CPP_FILE}"
#include "${PREFIX}_${NAME}.h"

static void* faust_new(t_symbol* s, int argc, t_atom* argv)
{
    t_faust* x = reinterpret_cast<t_faust*>(pd_new(faust_class));
    PdArgParser p(x, argc, argv);
//    p.initFloatArg("arg1", 1);
    return p.pd_obj();
}

extern "C" void setup_${PREFIX}0x2e${NAME}_tilde()
{
    internal_setup(gensym("${PREFIX}.${NAME}~"));
}

EOF

echo "\033[32mOk.\033[0m Add this line to ceammc/ext/src/CMakeLists.txt:"
echo ""
echo "    \033[36m ceammc_faust_extension(${PREFIX} ${NAME} $(echo $TEMPLATE | rev | cut -c 5- | rev))\033[0m"
echo ""


