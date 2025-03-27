#!/bin/sh

(
find ./rpi/_CPack_Packages -name '*.pd_linux' -or -name '*.so' | sort | while read so
do
	ldd $so | grep '=>' | cut -d' ' -f1 | sort | uniq | while read dep
	do
		dpkg -S "$dep" | cut -d: -f1
	done
done
) | grep -v 'pd-ceammc' | sort | uniq
