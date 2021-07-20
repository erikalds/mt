cd portaudio
mkdir out
cd out
cmake -DCMAKE_INSTALL_PREFIX=../../build -DCMAKE_BUILD_TYPE=Release -GNinja ../
ninja
ninja install
