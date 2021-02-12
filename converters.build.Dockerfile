## CONVERTERS ##
ENV CONVERTERSPATH /home/docker/converters
ENV CONVERTERSSOURCES /home/docker/converterssources

# Copy
WORKDIR $CONVERTERSSOURCES
COPY ./converters .

# Build converters
RUN mkdir build \
  && cd build \
  && cmake .. -DOpenCASCADE_DIR=$OCCPATH/lib/cmake/opencascade -DVTK_DIR=$VTKPATH/lib/cmake/vtk-9.0 -DCMAKE_INSTALL_PREFIX=$CONVERTERSPATH \
  && make -j "$(nproc)" \
  && make install
