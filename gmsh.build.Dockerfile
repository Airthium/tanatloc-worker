## GMSH ##
ENV GMSHPATH /home/programs/gmsh
ENV GMSHSOURCES /home/programs/gmshsources
ENV GMSHVERSION gmsh_4_11_1

WORKDIR /home/programs

# Clone gmsh directory
RUN git clone https://gitlab.onelab.info/gmsh/gmsh.git --depth 1 --branch ${GMSHVERSION} ${GMSHSOURCES}
WORKDIR $GMSHSOURCES

# Configure and build Gmsh
RUN mkdir build \
  && cd build \
  && cmake .. -DCMAKE_BUILD_TYPE=Release -DENABLE_FLTK=OFF -DCMAKE_PREFIX_PATH=${OCCPATH} -DCMAKE_INSTALL_PREFIX=${GMSHPATH} \
  && make -j 4 \
  && make install \
  && make clean
