## RELEASE ##
FROM ubuntu:22.04

LABEL maintainer="https://github.com/orgs/Airthium/people"

ENV DEBIAN_FRONTEND noninteractive

ENV BUILDER_OCCPATH /home/docker/occ
ENV BUILDER_GMSHPATH /home/docker/gmsh
ENV BUILDER_FREEFEMPATH /home/docker/freefem
ENV BUILDER_VTKPATH /home/docker/vtk
ENV BUILDER_CONVERTERSPATH /home/docker/converters

ENV OCCPATH /home/docker/occ
ENV GMSHPATH /home/docker/gmsh
ENV FREEFEMPATH /home/docker/freefem
ENV VTKPATH /home/docker/vtk
ENV CONVERTERSPATH /home/docker/converters

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

# User
RUN getent group users || groupadd -r users \
  && useradd -r -g users -d /home/user -s /sbin/nologin -c "Docker user" docker

# Copy from builder
COPY --from=builder --chown=docker:users $BUILDER_OCCPATH $OCCPATH
COPY --from=builder --chown=docker:users $BUILDER_GMSHPATH $GMSHPATH
COPY --from=builder --chown=docker:users $BUILDER_FREEFEMPATH $FREEFEMPATH
COPY --from=builder --chown=docker:users $BUILDER_VTKPATH $VTKPATH
COPY --from=builder --chown=docker:users $BUILDER_CONVERTERSPATH $CONVERTERSPATH

# Environment
USER docker

ENV PATH $GMSHPATH/bin:$FREEFEMPATH/bin:$CONVERTERSPATH/bin:$PATH
ENV LD_LIBRARY_PATH $OCCPATH/lib:$GMSHPATH/lib:$FREEFEMPATH/lib:$VTKPATH/lib:$CONVERTERSPATH/lib:$LD_LIBRARY_PATH
ENV FF_INCLUDEPATH $FREEFEMPATH/lib/ff++/4.9/idp
ENV FF_LOADPATH ./;;$FREEFEMPATH/lib/ff++/4.9/lib

WORKDIR /home/user