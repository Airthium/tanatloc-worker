##Catch2
ENV CATCH2PATH /home/programs/catch2
ENV CATCH2SOURCE /home/programs/catch2sources
ENV CATCH2VERSION v2.13.10

# Clone
RUN git clone https://github.com/catchorg/Catch2.git --depth 1 --branch ${CATCH2VERSION} ${CATCH2SOURCE}
WORKDIR $CATCH2SOURCE

# Build
RUN mkdir build \
  && cd build \
  && cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF -DCATCH_BUILD_TESTING=OFF -DCATCH_INSTALL_DOCS=OFF -DCATCH_ENABLE_WERROR=OFF -DCMAKE_INSTALL_PREFIX=${CATCH2PATH} \ 
  && make -j "$(nproc)" \
  && make install \
  && make clean

##TinyGLFT
ENV TINYGLTFPATH /home/programs/tinygltf
ENV TINYGLTFSOURCE /home/programs/tinygltfsources
ENV TINYGLTFVERSION v2.8.18

# Clone
RUN git clone https://github.com/syoyo/tinygltf.git --depth 1 --branch ${TINYGLTFVERSION} ${TINYGLTFSOURCE}
WORKDIR $TINYGLTFSOURCE

# Build
RUN mkdir build \
  && cd build \
  && cmake .. -DCMAKE_BUILD_TYPE=Release -DTINYGLTF_BUILD_LOADER_EXAMPLE=OFF -DCMAKE_INSTALL_PREFIX=${TINYGLTFPATH} \
  && make -j "$(nproc)" \
  && make install \
  && make clean

## CONVERTERS ##
ENV CONVERTERSPATH /home/programs/converters
ENV CONVERTERSSOURCES /home/programs/converterssources

# Copy
WORKDIR $CONVERTERSSOURCES
COPY ./tanatloc-converters .

# Build converters
RUN mkdir build \
  && cd build \
  && cmake .. -DBUILD_TESTING=OFF -DCMAKE_PREFIX_PATH="/usr/local/lib/cmake;${OCCPATH}/lib/cmake;${VTKPATH}/lib/cmake;${CATCH2PATH}/lib/cmake;${TINYGLTFPATH}/lib/cmake" -DTinyGLTF_INCLUDE_DIR=${TINYGLTFPATH}/include -DCMAKE_INSTALL_PREFIX=${CONVERTERSPATH} \
  && make -j "$(nproc)" \
  && make install \
  && make clean
