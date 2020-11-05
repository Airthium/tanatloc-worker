FROM ubuntu:20.10 as build

ENV OCCPATH /root/occ

WORKDIR $OCCPATH

# Copy OCC repository
COPY opencascade-7.4.0.tar.gz .
RUN tar -zxvf opencascade-7.4.0.tar.gz

# Configure and build OCC
RUN cd opencascade-7.4.0 \
  && mkdir build \
  && cd build \
  && cmake .. \
  && make -j "$(nproc)" \
  && make install
