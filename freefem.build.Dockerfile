## FREEFEM ##
ENV FREEFEMPATH /home/programs/freefem
ENV FREEFEMSOURCES /home/programs/freefemsources
ENV FREEFEMVERSIONNUMBER 4.13
ENV FREEFEMVERSION v$FREEFEMVERSIONNUMBER

WORKDIR /home/programs

# Clone FreeFEM repository
RUN git clone https://github.com/FreeFem/FreeFem-sources.git --depth 1 --branch ${FREEFEMVERSION} ${FREEFEMSOURCES}
WORKDIR $FREEFEMSOURCES

# Configure and build FreeFEM
RUN autoreconf -i \
	&& ./configure --enable-download --enable-optim --enable-generic --prefix=${FREEFEMPATH} \
	&& ./3rdparty/getall -a \
	&& cd 3rdparty/ff-petsc \
	&& make petsc-slepc \
	&& cd - \
	&& ./reconfigure \
	&& make -j1 \
	&& make install \
	&& make clean
