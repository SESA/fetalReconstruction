FROM ubuntu:xenial

RUN apt-get update -y && \
    apt-get install -y git automake autoconf \
    build-essential cmake libtbb-dev libgsl0-dev \
    libboost-program-options-dev \
    libboost-filesystem-dev \
    libboost-serialization-dev \
    libboost-thread-dev
    
RUN git clone --recursive https://github.com/sesa/fetalReconstruction.git /fe
WORKDIR /fe
RUN cmake -DCMAKE_BUILD_TYPE=Release ./source 
RUN make -j 

ENTRYPOINT ["/fe/bin/reconstruction_GPU2"]
