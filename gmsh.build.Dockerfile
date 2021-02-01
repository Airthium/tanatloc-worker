## GMSH ##
FROM ubuntu:20.10 as build

ENV GMSHPATH /root/gmsh
ENV GMSHSOURCES /root/gmshsources

WORKDIR /root

# Copy gmsh directory
COPY gmsh.tar.gz .
RUN mkdir $GMSHSOURCES && tar -zxf gmsh.tar.gz -C $GMSHSOURCES --strip-components 1

WORKDIR $GMSHSOURCES

# Configure and build Gmsh
RUN mkdir build \
  && cd build \
  && cmake .. -DENABLE_FLTK=OFF -DCMAKE_INSTALL_PREFIX=$GMSHPATH \
  && make -j "$(nproc)" \
  && make install
