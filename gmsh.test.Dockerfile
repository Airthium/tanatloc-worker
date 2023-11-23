FROM ubuntu:22.04 as build

WORKDIR $GMSHSOURCES

# Test gmsh
RUN cd gmsh/build \
	&& make test
