#!/bin/bash

echo "\${CEAMMC_COVERAGE} = \"${CEAMMC_COVERAGE}\""

if [[ ${CEAMMC_COVERAGE} == "ON" ]] 
then 
	echo "Upload coverage to coveralls.io ..."
    coveralls --exclude . --include ceammc/ext/src/lib --root .. --build-root . --gcov-options '\-lp' > /dev/null
else
	echo "Skip coveralls.io ..."
fi

