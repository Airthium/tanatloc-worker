## GMSH ##
ENV GMSHPATH /home/programs/gmsh
ENV GMSHSOURCES /home/programs/gmshsources

WORKDIR /home/programs

# Copy gmsh directory
COPY gmsh.tar.gz .
RUN mkdir $GMSHSOURCES && tar -zxf gmsh.tar.gz -C $GMSHSOURCES --strip-components 1

WORKDIR $GMSHSOURCES

# Configure and build Gmsh
RUN mkdir build \
  && cd build \
  && cmake .. -DENABLE_FLTK=OFF -DCMAKE_PREFIX_PATH=$OCCPATH -DCMAKE_INSTALL_PREFIX=$GMSHPATH \
  && make -j "$(nproc)" \
  && make install
