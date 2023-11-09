## VTK ##
ENV VTKPATH /home/programs/vtk
ENV VTKSOURCES /home/programs/vtksources
ENV VTKVERSION v9.2.6

WORKDIR /home/programs

# Clone VTK repository
RUN git clone https://gitlab.kitware.com/vtk/vtk.git --depth 1 --branch ${VTKVERSION} ${VTKSOURCES}
WORKDIR $VTKSOURCES

# Configure and build VTK
RUN mkdir build \
  && cd build \
  && cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${VTKPATH} \
  && make -j "$(nproc)" \
  && make install \
  && make clean
