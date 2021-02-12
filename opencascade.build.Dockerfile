## OCC ##
ENV OCCPATH /home/docker/occ
ENV OCCSOURCES /home/docker/occsources

WORKDIR /home/docker

# Copy OCC repository
COPY opencascade-7.4.0.tar.gz .
RUN mkdir $OCCSOURCES && tar -zxf opencascade-7.4.0.tar.gz -C $OCCSOURCES --strip-components 1

WORKDIR $OCCSOURCES

# Configure and build OCC
RUN mkdir build \
  && cd build \
  && cmake .. -DINSTALL_DIR=$OCCPATH \
  && make -j "$(nproc)" \
  && make install
