FROM ubuntu:20.10 as build

WORKDIR $GMSHSOURCES

# Test gmsh
RUN cd gmsh/build \
  && make test
