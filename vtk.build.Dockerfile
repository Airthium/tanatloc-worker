## VTK ##
ENV VTKPATH /home/programs/vtk
ENV VTKSOURCES /home/programs/vtksources

WORKDIR /home/programs

# Copy VTK repository
COPY VTK-9.1.0.tar.gz .
RUN mkdir $VTKSOURCES && tar -zxf VTK-9.1.0.tar.gz -C $VTKSOURCES --strip-components 1

WORKDIR $VTKSOURCES

# Configure and build VTK
RUN mkdir build \
  && cd build \
  && cmake .. -DCMAKE_INSTALL_PREFIX=$VTKPATH \
  && make -j "$(nproc)" \
  && make install
