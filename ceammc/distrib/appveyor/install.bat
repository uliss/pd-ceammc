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
bash -lc "pacman -S --needed --noconfirm mingw-w64-x86_64-toolchain cmake make patch mingw-w64-x86_64-libtool"
bash -lc "mkdir build"
bash -lc "cd build"
bash -lc "which cmake"
bash -lc "/mingw32/bin/cmake -G 'MSYS Makefiles' .."

