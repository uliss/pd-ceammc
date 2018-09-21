@echo off

cd %APPVEYOR_BUILD_FOLDER%

echo Compiler: %COMPILER%
echo Architecture: %MSYS2_ARCH%
echo Platform: %PLATFORM%
echo MSYS2 directory: %MSYS2_DIR%
echo MSYS2 system: %MSYSTEM%
echo Bits: %BIT%

mkdir %APPVEYOR_BUILD_FOLDER%\build
set TMPDIR=%APPVEYOR_BUILD_FOLDER%\build

dir

@echo on
\msys64\mingw32.exe

pacman -S --needed --noconfirm pacman-mirrors
pacman -S --needed --noconfirm git
pacman -Syu --noconfirm
pacman -S --needed --noconfirm cmake make patch

pwd
ls -l
cmake ..

