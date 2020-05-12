FROM ubuntu:20.04 as build

ENV GMSHPATH /root/gmsh

WORKDIR $GMSHPATH

# Copy gmsh directory
COPY gmsh.tar.gz .
RUN tar xf gmsh.tar.gz

# Configure and build Gmsh
RUN cd gmsh \
  && mkdir build \
  && cd build \
  && cmake .. \
  && make -j "$(nproc)" \
  && make install

# Fix shared library missing using
# export LD_LIBRARY_PATH=/usr/local/lib
