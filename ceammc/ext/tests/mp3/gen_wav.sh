#!/bin/bash

c=1

function gen_mp3() {
    fr=$1
    ch=$2
    sr=$3
    br=$4

    fname="test_${ch}ch_${sr}_$br.wav"
    out=${fname%.wav}.mp3
    sox -n -c $ch -r $sr $fname synth 1 sine $fr
    if [ $br == 'vbr' ]
    then
        lame -V 4 $fname $out 
    else
        lame -b $br $fname $out
    fi

    # sox mp3 decoder outputs strange results
    ffmpeg -i $out ${out}.wav
    offset=1000
    len=32
    out_dat=${out%.mp3}_off${offset}_len${len}.dat
    sox $out.wav -t dat - | sed 1,2d | sed 1,1000d | head -32 | awk '{print $2,$3}' > ${out_dat}
    rm $out.wav
}

gen_mp3 400 1 44100 192
gen_mp3 400 2 44100 192
gen_mp3 500 1 12000 128 
gen_mp3 550 2 12000 128
gen_mp3 600 1 24000 vbr
gen_mp3 660 2 24000 vbr


