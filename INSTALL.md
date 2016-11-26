### Installation

## MacOS

Install Homebrew: http://brew.sh/


install libs
``
brew install cmake boost fftw portaudio glib
``

clone and make
``
git clone https://github.com/uliss/pure-data.git
cd pure-data
git branch ceammc
cd ..
mkdir build
cd build
cmake -DWITH_PORTAUDIO=ON -DWITH_PORTMIDI=ON ../pure-data
make
make app
make dmg
``
