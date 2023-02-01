FROM ubuntu:23.04 as build

WORKDIR $GMSHSOURCES

# Test gmsh
RUN cd gmsh/build \
  && make test
