# Crosscompiling for Windows on MacOSX

First, we need to install mingw-w64

`brew install mingw-w32`

then install needed libraries

```sh
HOST=i686-w64-mingw32
PREFIX=/usr/local/mingw/${HOST}
export PKG_CONFIG_PATH=/usr/local/mingw/i686-w64-mingw32/lib/pkgconfig
export PATH=/usr/local/mingw/i686-w64-mingw32:/usr/local/mingw/bin:$PATH

# install libogg
brew unpack libogg
cd libogg-*
./configure --host=${HOST} --prefix=${PREFIX} \
     --with-sysroot=/usr/local/mingw/i686-w64-mingw32
make  
make install
cd ..

# install libvorbis
brew unpack libvorbis
cd libvorbis-*
./configure --host=${HOST} --prefix=${PREFIX} \
      --with-sysroot=/usr/local/mingw/i686-w64-mingw32
make 
make install
cd ..

# install FLAC
brew unpack flac
cd flac-*
./configure --disable-cpplibs --disable-oggtest \
      --host=${HOST} --prefix=${PREFIX} \
      --with-sysroot=/usr/local/mingw/i686-w64-mingw32
make 
make install
cd ..

# install libsndfile
brew unpack libsndfile
cd libsndfile-*
./configure --host=${HOST} --prefix=${PREFIX} \
      --with-sysroot=/usr/local/mingw/i686-w64-mingw32
make 
make install
cd ..

# install libffi
brew unpack libffi
cd libffi-*
./configure --host=${HOST} --prefix=${PREFIX} \
      --with-sysroot=/usr/local/mingw/i686-w64-mingw32
make 
make install
cd ..

# install libiconv
wget https://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.15.tar.gz
tar zxvf libiconv-1.15.tar.gz
cd libiconv-1*
./configure --host=${HOST} --prefix=${PREFIX} \
      --enable-shared=yes --enable-static=yes \
      --with-sysroot=/usr/local/mingw/i686-w64-mingw32
make 
make install
cd ..

# install xz
brew unpack xz
cd xz*
./configure --host=${HOST} --prefix=${PREFIX} \
      --enable-shared=yes --enable-static=yes \
      --with-sysroot=/usr/local/mingw/i686-w64-mingw32
make 
make install
cd ..

# install libxml2
brew unpack libxml2
cd libxml*
./configure --host=${HOST} --prefix=${PREFIX} \
      --disable-shared --enable-static=yes \
      --with-sysroot=/usr/local/mingw/i686-w64-mingw32
make 
make install
cd ..

```

then configure cmake for crosscompiling:

```sh
mkdir build
cp ceammc/distrib/win/Toolchain-mingw32.cmake build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=Toolchain-mingw32.cmake ..
make
make install
```

to make installer you have to install InnoSetup with wine.
```sh 
brew install wine
```

after that you can do *(after make install step):*
```sh
wine PATH_TO_ISCC.exe pd.iss
```
