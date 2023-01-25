##Catch2
ENV CATCH2SOURCE /home/programs/catch2

# Copy
RUN git clone https://github.com/catchorg/Catch2.git -b v2.x $CATCH2SOURCE
WORKDIR $CATCH2SOURCE

# Build
RUN mkdir build \
  && cd build \
  && cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF -DCATCH_BUILD_TESTING=OFF -DCATCH_INSTALL_DOCS=OFF -DCATCH_ENABLE_WERROR=OFF \ 
  && make -j "$(nproc)" \
  && make install

##TinyGLFT
ENV TINYGLTFSOURCE /home/programs/tinygltf

# Copy
RUN git clone https://github.com/syoyo/tinygltf.git $TINYGLTFSOURCE
WORKDIR $TINYGLTFSOURCE

# Build
RUN mkdir build \
  && cd build \
  && cmake .. -DCMAKE_BUILD_TYPE=Release -DTINYGLTF_BUILD_LOADER_EXAMPLE=OFF  \
  && make -j "$(nproc)" \
  && make install

## CONVERTERS ##
ENV CONVERTERSPATH /home/programs/converters
ENV CONVERTERSSOURCES /home/programs/converterssources

# Copy
WORKDIR $CONVERTERSSOURCES
COPY ./tanatloc-converters .

# Build converters
RUN mkdir build \
  && cd build \
  && cmake .. -DBUILD_TESTING=OFF -DCMAKE_PREFIX_PATH=/usr/local/lib/cmake -DOpenCASCADE_DIR=$OCCPATH/lib/cmake/opencascade -DVTK_DIR=$VTKPATH/lib/cmake/vtk-9.1 -DCMAKE_INSTALL_PREFIX=$CONVERTERSPATH \
  && make -j "$(nproc)" \
  && make install
