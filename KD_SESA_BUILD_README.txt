#dependencies
FLTK version 1.1.10
VTK 5 or 6
Boost, no earlier than version 1.48
GNU Scientific Library
CMAKE

#set up environment in .bashrc file
export IRTK_DIR=/home/handong/build-irtk
export PATH="$IRTK_DIR/bin:$PATH"
export PATH=/usr/local/cuda-7.0/bin:$PATH
export LD_LIBRARY_PATH=/usr/local/cuda-7.0/lib64
export PATH=/usr/local/cuda-6.5/bin:$PATH
export LD_LIBRARY_PATH=/usr/local/cuda-6.5/lib64:$LD_LIBRARY_PATH

#cmake instructions
add /usr/local/cuda-6.5/samples to source/reconstructionGPU2/CMakeLists.txt
cd source
mkdir build
cmake -DCMAKE_C_COMPILER=gcc-4.8 -DCMAKE_CXX_COMPILER=g++-4.8 -DCMAKE_BUILD_TYPE=Debug ..
edit CMakeCache.txt and set BUILD_TESTING:BOOL = OFF
make -j16


