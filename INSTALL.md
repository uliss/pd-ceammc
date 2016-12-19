### Installation

## MacOS

Install Homebrew: http://brew.sh/


Install libs:

```
brew install cmake boost gettext
brew install --universal portaudio fftw glib libsndfile
```

clone and build:

```
git clone https://github.com/uliss/pure-data.git
cd pure-data
git fetch origin
git checkout ceammc
cd ..
mkdir build
cd build
cmake -DWITH_PORTAUDIO=ON -DWITH_PORTMIDI=ON ../pure-data
make
make app
make dmg
```

Application bundle in ./dist directory and DMG in root of build directory.
