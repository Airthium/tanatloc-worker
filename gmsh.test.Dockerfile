FROM ubuntu:20.04 as build

ENV GMSHPATH /root/gmsh

WORKDIR $GMSHPATH

# RUN cd gmsh/build \
#   && make test
