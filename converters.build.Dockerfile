## CONVERTERS ##
ENV CONVERTERSPATH /root/converters
ENV CONVERTERSSOURCES /root/converterssources

# Copy
WORKDIR $CONVERTERSSOURCES
COPY ./converters .

# Build converters
RUN mkdir build \
  && cd build \
  && cmake .. -DOpenCASCADE_DIR=/root/occ/lib/cmake/opencascade -DVTK_DIR=/root/vtk/lib/cmake/vtk-9.0 -DCMAKE_INSTALL_PREFIX=$CONVERTERSPATH \
  && make -j "$(nproc)" \
  && make install
