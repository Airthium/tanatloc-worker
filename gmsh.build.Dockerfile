## GMSH ##
ENV GMSHPATH /home/programs/gmsh
ENV GMSHSOURCES /home/programs/gmshsources

WORKDIR /home/programs

# Copy gmsh directory
COPY gmsh-4.10.1-source.tgz .
RUN mkdir $GMSHSOURCES && tar -zxf gmsh-4.10.1-source.tgz -C $GMSHSOURCES --strip-components 1

WORKDIR $GMSHSOURCES

# Configure and build Gmsh
RUN mkdir build \
  && cd build \
  && cmake .. -DCMAKE_BUILD_TYPE=Release -DENABLE_FLTK=OFF -DCMAKE_PREFIX_PATH=$OCCPATH -DCMAKE_INSTALL_PREFIX=$GMSHPATH \
  && make -j "$(nproc)" \
  && make install
