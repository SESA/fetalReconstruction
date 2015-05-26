add /usr/local/cuda-6.5/samples to source/reconstructionGPU2/CMakeLists.txt
cd source
mkdir build
cmake -DCMAKE_C_COMPILER=gcc-4.8 -DCMAKE_CXX_COMPILER=g++-4.8 -DCMAKE_BUILD_TYPE=Debug ..
edit CMakeCache.txt and set BUILD_TESTING:BOOL = OFF
make -j16


