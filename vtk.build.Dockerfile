## VTK ##
ENV VTKPATH /root/vtk
ENV VTKSOURCES /root/vtksources

WORKDIR /root

# Copy VTK repository
COPY VTK-9.0.1.tar.gz .
RUN mkdir $VTKSOURCES && tar -zxf VTK-9.0.1.tar.gz -C $VTKSOURCES --strip-components 1

WORKDIR $VTKSOURCES

# Configure and build VTK
RUN mkdir build \
  && cd build \
  && cmake .. -DCMAKE_INSTALL_PREFIX=$VTKPATH \
  && make -j "$(nproc)" \
  && make install
