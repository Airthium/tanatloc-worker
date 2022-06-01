## OCC ##
ENV OCCPATH /home/programs/occ
ENV OCCSOURCES /home/programs/occsources

WORKDIR /home/programs

# Copy OCC repository
COPY opencascade-7.6.0.tgz .
RUN mkdir $OCCSOURCES && tar -zxf opencascade-7.6.0.tgz -C $OCCSOURCES --strip-components 1

WORKDIR $OCCSOURCES

# Configure and build OCC
RUN mkdir build \
  && cd build \
  && cmake .. -DCMAKE_BUILD_TYPE=Release -DUSE_RAPIDJSON=ON -DINSTALL_DIR=$OCCPATH \
  && make -j "$(nproc)" \
  && make install
