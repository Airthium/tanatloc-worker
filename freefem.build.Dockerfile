## FREEFEM ##
FROM ubuntu:20.10 as builder

ENV FREEFEMPATH /root/freefem
ENV FREEFEMSOURCES /root/freefemsources

WORKDIR /root

# Copy FreeFEM repository
COPY FreeFem-sources-4.8.tar.gz .
RUN mkdir $FREEFEMSOURCES && tar -zxf FreeFem-sources-4.8.tar.gz -C $FREEFEMSOURCES --strip-components 1

WORKDIR $FREEFEMSOURCES

# Configure and build FreeFEM
RUN autoreconf -i \
    && ./configure --enable-download --enable-optim --enable-generic --prefix=$FREEFEMPATH \
    && ./3rdparty/getall -o PETSc -a

RUN cd 3rdparty/ff-petsc \
    && sed -i'.bak' 's#download-mpich#download-mpich=http://www.mpich.org/static/downloads/3.4rc1/mpich-3.4rc1.tar.gz#' Makefile \
    && make petsc-slepc

RUN ./reconfigure \
    && make -j "$(nproc)"

RUN make install
