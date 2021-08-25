FROM ubuntu:21.10 as build

WORKDIR $GMSHSOURCES

# Test gmsh
RUN cd gmsh/build \
  && make test
