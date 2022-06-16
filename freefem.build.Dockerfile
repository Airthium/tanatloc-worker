## FREEFEM ##
ENV FREEFEMPATH /home/programs/freefem
ENV FREEFEMSOURCES /home/programs/freefemsources

WORKDIR /home/programs

# Copy FreeFEM repository
COPY FreeFem-sources-4.11.tar.gz .
RUN mkdir $FREEFEMSOURCES && tar -zxf FreeFem-sources-4.11.tar.gz -C $FREEFEMSOURCES --strip-components 1

WORKDIR $FREEFEMSOURCES

# Configure and build FreeFEM
RUN autoreconf -i \
	&& ./configure --enable-download --enable-optim --enable-generic --prefix=$FREEFEMPATH \
	&& ./3rdparty/getall -o PETSc -a \
	&& cd 3rdparty/ff-petsc \
	&& sed -i'.bak' 's#download-mpich#download-mpich=http://www.mpich.org/static/downloads/3.4rc1/mpich-3.4rc1.tar.gz#' Makefile \
	&& make petsc-slepc \
	&& cd - \
	&& ./reconfigure \
	&& make -j "$(nproc)" \
	&& make install
