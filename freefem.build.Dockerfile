## FREEFEM ##
ENV FREEFEMPATH /home/docker/freefem
ENV FREEFEMSOURCES /home/docker/freefemsources

WORKDIR /home/docker

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
    && make -j "$(nproc)" \
    && make install
