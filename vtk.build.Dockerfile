FROM ubuntu:20.10 as build

ENV VTKPATH /root/vtk

WORKDIR $VTKPATH

# Copy VTK repository
COPY VTK-9.0.1.tar.gz .
RUN tar -zxvf VTK-9.0.1.tar.gz

# Configure and build VTK
RUN cd VTK-9.0.1 \
  && mkdir build \
  && cd build \
  && cmake .. \
  && make -j "$(nproc)" \
  && make install
