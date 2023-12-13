## RELEASE ##
FROM ubuntu:20.04

LABEL maintainer="https://github.com/orgs/Airthium/people"

ENV DEBIAN_FRONTEND noninteractive

ENV OCCPATH /home/programs/occ
ENV GMSHPATH /home/programs/gmsh
ENV FREEFEMPATH /home/programs/freefem
ENV VTKPATH /home/programs/vtk
ENV TINYGLTFPATH /home/programs/tinygltf
ENV CONVERTERSPATH /home/programs/converters

# Install packages
RUN apt update \
	&& apt upgrade -yq \
	&& apt install -yq \
	libfontconfig1 libgl1-mesa-dev \
	libarpack2-dev \
	libopenblas-dev libhdf5-dev \
	libgsl-dev libfftw3-dev \
	libnlopt-dev libumfpack5 \
	python3-paraview \
	&& apt autoremove \
	&& apt clean \
	&& rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# Copy from builder
COPY --from=builder $OCCPATH $OCCPATH
COPY --from=builder $GMSHPATH $GMSHPATH
COPY --from=builder $FREEFEMPATH $FREEFEMPATH
COPY --from=builder $VTKPATH $VTKPATH
COPY --from=builder $CONVERTERSPATH $CONVERTERSPATH

# Environment
ENV PATH $GMSHPATH/bin:$FREEFEMPATH/bin:$CONVERTERSPATH/bin:$PATH
ENV LD_LIBRARY_PATH $OCCPATH/lib:$GMSHPATH/lib:$FREEFEMPATH/lib:$VTKPATH/lib:$TINYGLTFPATH/lib:$CONVERTERSPATH/lib:$LD_LIBRARY_PATH
ENV FF_INCLUDEPATH $FREEFEMPATH/lib/ff++/$FREEFEMVERSIONNUMBER/idp
ENV FF_LOADPATH ./;;$FREEFEMPATH/lib/ff++/$FREEFEMVERSIONNUMBER/lib
