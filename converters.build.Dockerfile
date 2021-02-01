FROM ubuntu:20.10

LABEL maintainer="https://github.com/orgs/Airthium/people"

ENV DEBIAN_FRONTEND noninteractive

ENV CONVERTERSPATH /root/converters
ENV CONVERTERSSOURCES /root/converterssources

# Install packages
RUN apt-get update \
  && apt-get install -yq \
  cmake \
  g++ \
  libfftw3-dev libgl1-mesa-dev libgsl-dev \
  libhdf5-dev liblapack-dev libnlopt-dev \
  libopenblas-dev \
  mesa-common-dev \
  pkg-config \
  && apt-get autoremove \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# Copy
WORKDIR $CONVERTERSSOURCES

COPY ./converters .

# Copy from builder
COPY --from=builder $OCCPATH $OCCPATH
COPY --from=builder $GMSHPATH $GMSHPATH
COPY --from=builder $FREEFEMPATH $FREEFEMPATH
COPY --from=builder $VTKPATH $VTKPATH

# Build converters
RUN mkdir build \
  && cd build \
  && cmake .. -DCMAKE_INSTALL_PREFIX=$CONVERTERSPATH \
  && make -j "$(nproc)" \
  && make install

# Cleanup
RUN rm -rf $CONVERTERSSOURCES

# Environment
WORKDIR /root
RUN export PATH=$GMSHPATH/bin:$FREEFEMPATH/bin:$CONVERTERSPATH/bin:$PATH
