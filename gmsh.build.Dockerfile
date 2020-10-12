FROM ubuntu:20.10 as build

ENV GMSHPATH /root/gmsh

WORKDIR $GMSHPATH

# Copy gmsh directory
COPY gmsh.tar.gz .
RUN tar -zxvf gmsh.tar.gz

# Configure and build Gmsh
RUN cd gmsh \
  && mkdir build \
  && cd build \
  && cmake .. -DENABLE_FLTK=OFF \
  && make -j "$(nproc)" \
  && make install

ENV LD_LIBRARY_PATH /usr/local/lib
