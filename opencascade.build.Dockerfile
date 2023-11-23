## OCC ##
ENV OCCPATH /home/programs/occ
ENV OCCSOURCES /home/programs/occsources
ENV OCCVERSION master

WORKDIR /home/programs

# Clone OCC repository
RUN git clone https://github.com/Open-Cascade-SAS/OCCT.git --depth 1 --branch ${OCCVERSION} ${OCCSOURCES}
WORKDIR $OCCSOURCES

# Configure and build OCC
RUN mkdir build \
	&& cd build \
	&& cmake .. -DCMAKE_BUILD_TYPE=Release -DUSE_RAPIDJSON=ON -DINSTALL_DIR=${OCCPATH} \
	&& make -j "$(nproc)" \
	&& make install \
	&& make clean
