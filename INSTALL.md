### Installation

## MacOS

Install Homebrew: http://brew.sh/


Install libs:

```
brew install cmake boost fftw portaudio glib gettext
```

clone and build:

```
git clone https://github.com/uliss/pure-data.git
cd pure-data
git checkout ceammc
cd ..
mkdir build
cd build
cmake -DWITH_PORTAUDIO=ON -DWITH_PORTMIDI=ON ../pure-data
make
make app
make dmg
```
