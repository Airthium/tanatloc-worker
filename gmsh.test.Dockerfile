FROM ubuntu:20.04 as build

WORKDIR $GMSHSOURCES

# Test gmsh
RUN cd gmsh/build \
	&& make test
