#!/bin/bash

IMGDIR="../icons/buttonbar/cyan"
DARKDIR="../icons/buttonbar/dark"
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
    img_dark="${DARKDIR}/${shortname}.png"
    echo "set ${shortname}data_light {$(base64 $i)}" >> ${OUT}
    echo "set ${shortname}data_dark {$(base64 $img_dark)}" >> ${OUT}
    echo "image create photo btn_image_${shortname}_light -data \$${shortname}data_light" >> ${OUT}
    echo "image create photo btn_image_${shortname}_dark -data \$${shortname}data_dark" >> ${OUT}
    echo "" >> ${OUT}
done

echo '::pdwindow::debug "\[ceammc\]: ceammc_buttonbar-plugin loaded\\n"' >> ${OUT}
