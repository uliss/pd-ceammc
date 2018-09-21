@echo off

cd %APPVEYOR_BUILD_FOLDER%

echo Compiler: %COMPILER%
echo Architecture: %MSYS2_ARCH%
echo Platform: %PLATFORM%
echo MSYS2 directory: %MSYS2_DIR%
echo MSYS2 system: %MSYSTEM%
echo Bits: %BIT%

mkdir %APPVEYOR_BUILD_FOLDER%\build

@echo on
set "PATH=C:\%MSYS2_DIR%\%MSYSTEM%\bin;C:\%MSYS2_DIR%\usr\bin;%PATH%"
echo Path: %PATH%
bash -lc "pacman -S --needed --noconfirm pacman-mirrors"
bash -lc "pacman -S --needed --noconfirm git"
REM Update
bash -lc "pacman -Syu --noconfirm"

REM build tools
bash -lc "pacman -S --needed --noconfirm mingw-w64-${MSYS2_ARCH}-toolchain mingw-w64-${MSYS2_ARCH}-cmake make patch mingw-w64-${MSYS2_ARCH}-libtool"
bash -lc "cd ${APPVEYOR_BUILD_FOLDER}/build && cmake -G 'MSYS Makefiles' .."

