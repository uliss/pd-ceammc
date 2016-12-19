#!/bin/bash

IMGDIR="../icons/buttonbar/cyan"
OUT="$1"

if [ $# -ne 1 ]
then
    echo "Usage: $0 OUTPUT"
    exit 1
fi

cat "buttonbar.tmpl.tcl" > ${OUT}

for i in ${IMGDIR}/*.gif
do
    name=${i%.gif}
    shortname=$(basename $name)
    echo "set ${shortname}data {$(base64 $i)}" >> ${OUT}
    echo "image create photo buttonimage${shortname} -data \$${shortname}data" >> ${OUT}
    echo "" >> ${OUT}
done

echo '::pdwindow::debug "\[ceammc\]: ceammc_buttonbar-plugin loaded\\n"' >> ${OUT}
