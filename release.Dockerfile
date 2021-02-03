## RELEASE ##
FROM ubuntu:20.10

LABEL maintainer="https://github.com/orgs/Airthium/people"

ENV DEBIAN_FRONTEND noninteractive

ENV BUILDER_OCCPATH /root/occ
ENV BUILDER_GMSHPATH /root/gmsh
ENV BUILDER_FREEFEMPATH /root/freefem
ENV BUILDER_VTKPATH /root/vtk
ENV BUILDER_CONVERTERSPATH /root/converters

ENV OCCPATH $BUILDER_OCCPATH
ENV GMSHPATH $BUILDER_GMSHPATH
ENV FREEFEMPATH $BUILDER_FREEFEMPATH
ENV VTKPATH $BUILDER_VTKPATH
ENV CONVERTERSPATH $BUILDER_CONVERTERSPATH

# Install packages
RUN apt update \
  && apt upgrade -yq

RUN apt install -yq \
  libfontconfig1 libgl1-mesa-dev libhdf5-dev liblapack-dev

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
WORKDIR /root
ENV PATH $GMSHPATH/bin:$FREEFEMPATH/bin:$CONVERTERSPATH/bin:$PATH
ENV LD_LIBRARY_PATH $OCCPATH/lib:$GMSHPATH/lib:$FREEFEMPATH/lib:$VTKPATH/lib:$CONVERTERSPATH/lib:$LD_LIBRARY_PATH
