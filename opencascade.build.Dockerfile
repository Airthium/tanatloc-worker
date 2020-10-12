# OpenCascade 7.4.0
FROM ubuntu:20.10 as build

ENV OCCPATH /root/occ

WORKDIR $OCCPATH

# Copy OCC repository
# RUN wget https://github.com/tanatloc/Gmsh-docker/releases/download/0/opencascade-7.4.0.tgz
# RUN tar xf opencascade-7.4.0.tgz

COPY opencascade-7.4.0.tar.gz .
RUN tar -zxvf opencascade-7.4.0.tar.gz

# Configure and build OCC
RUN cd opencascade-7.4.0 \
  && mkdir build \
  && cd build \
  && cmake ..

RUN cd opencascade-7.4.0/build \
  && make -j "$(nproc)"

RUN cd opencascade-7.4.0/build \
  && make install
