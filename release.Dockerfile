## RELEASE ##
FROM ubuntu:20.04

LABEL maintainer="https://github.com/orgs/Airthium/people"

ENV DEBIAN_FRONTEND noninteractive

ENV BUILDER_OCCPATH /home/programs/occ
ENV BUILDER_GMSHPATH /home/programs/gmsh
ENV BUILDER_FREEFEMPATH /home/programs/freefem
ENV BUILDER_VTKPATH /home/programs/vtk
ENV BUILDER_CONVERTERSPATH /home/programs/converters

ENV OCCPATH /home/programs/occ
ENV GMSHPATH /home/programs/gmsh
ENV FREEFEMPATH /home/programs/freefem
ENV VTKPATH /home/programs/vtk
ENV CONVERTERSPATH /home/programs/converters

# Install packages
RUN apt update \
  && apt upgrade -yq

RUN apt install -yq \
  libfontconfig1 libgl1-mesa-dev \
  libarpack2-dev \
  libopenblas-dev libhdf5-dev \
  libgsl-dev libfftw3-dev \
  libnlopt-dev libumfpack5

RUN apt autoremove \
  && apt clean \
  && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# Copy from builder
COPY --from=builder $BUILDER_OCCPATH $OCCPATH
COPY --from=builder $BUILDER_GMSHPATH $GMSHPATH
COPY --from=builder $BUILDER_FREEFEMPATH $FREEFEMPATH
COPY --from=builder $BUILDER_VTKPATH $VTKPATH
COPY --from=builder $BUILDER_CONVERTERSPATH $CONVERTERSPATH

# Environment
ENV PATH $GMSHPATH/bin:$FREEFEMPATH/bin:$CONVERTERSPATH/bin:$PATH
ENV LD_LIBRARY_PATH $OCCPATH/lib:$GMSHPATH/lib:$FREEFEMPATH/lib:$VTKPATH/lib:$CONVERTERSPATH/lib:$LD_LIBRARY_PATH
ENV FF_INCLUDEPATH $FREEFEMPATH/lib/ff++/4.9/idp
ENV FF_LOADPATH ./;;$FREEFEMPATH/lib/ff++/4.9/lib
