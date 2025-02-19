@echo off

cd %APPVEYOR_BUILD_FOLDER%

echo Compiler: %COMPILER%
echo Architecture: %MSYS2_ARCH%
echo Platform: %PLATFORM%
echo MSYS2 directory: %MSYS2_DIR%
echo MSYS2 system: %MSYSTEM%
echo Double precision: %DOUBLE_PRECISION%

@echo on
set "PATH=C:\%MSYS2_DIR%\%MSYSTEM%\bin;C:\%MSYS2_DIR%\usr\bin;%PATH%"
bash -lc "pacman -Sy --needed --noconfirm pacman-mirrors"
REM Update
bash -lc "pacman -Syu --noconfirm"

REM build tools
bash -lc "pacman -Sy --needed --noconfirm mingw-w64-${MSYS2_ARCH}-{binutils,crt,tools-git,compiler-rt} git make patch"
REM dependencies
bash -lc "pacman -Sy --needed --noconfirm mingw-w64-${MSYS2_ARCH}-{cmake,ninja,glib2,libmodplug,tcllib,tcl,tk,tklib,portaudio,fftw,libsndfile,boost,dlfcn,armadillo,llvm,polly,luajit}"

bash -lc "echo ${APPVEYOR_BUILD_FOLDER}"
bash -lc "ls ${APPVEYOR_BUILD_FOLDER}"
bash -lc "mkdir ${APPVEYOR_BUILD_FOLDER}/build"
bash -lc "cd ${APPVEYOR_BUILD_FOLDER}/build && cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DWITH_FFTW=ON -DWITH_BENCHMARK=OFF -DENABLE_TESTS=OFF -DWITH_DOUBLE_PRECISION=${DOUBLE_PRECISION} -DWITH_EXT_FLEXT=OFF -DWITH_FAUST=OFF -DCMAKE_INSTALL_PREFIX=/opt/local/release/pd ${APPVEYOR_BUILD_FOLDER}"
bash -lc "ls ${APPVEYOR_BUILD_FOLDER}/build"
bash -lc "cd ${APPVEYOR_BUILD_FOLDER}/build && ninja install && ./fix_dll_deps.sh && ./win_fix_pddoc_key_modifiers.sh"
bash -lc "cd ${APPVEYOR_BUILD_FOLDER}/build && ninja inno && ninja ceammc_lib"
