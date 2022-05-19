cmake_minimum_required(VERSION 3.0)

project(Tanatloc)

## Debug
option(DEBUG "Print debug informations" OFF)
if (DEBUG)
  add_definitions(-DDEBUG_MODE)
else()
  remove_definitions(-DDEBUG_MODE)
endif()

## Build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

## LIBRARIES
# VTK
find_package(VTK COMPONENTS
  IOXML
REQUIRED)
if (VTK_FOUND)
  include_directories(${VTK_INCLUDE_DIRS})
  set(LIBS ${LIBS} ${VTK_LIBRARY_DIRS})
  set(VTK_LIBS ${VTK_LIBRARIES})
endif (VTK_FOUND)

# Catch2
find_package(Catch2 REQUIRED)

# OpenCASCADE
find_package(OpenCASCADE REQUIRED)
if (OpenCASCADE_FOUND)
  include_directories(${OpenCASCADE_INCLUDE_DIR})
  set(LIBS ${LIBS} ${OpenCASCADE_LIBRARY_DIR})
  set(OCC_LIBS ${OpenCASCADE_LIBRARIES})
endif (OpenCASCADE_FOUND)

# Logger library
AUX_SOURCE_DIRECTORY(src/logger LOGGER_SOURCE)

# DXFLib library
AUX_SOURCE_DIRECTORY(src/dxflib DXFLib_SOURCE)

# DXF library
AUX_SOURCE_DIRECTORY(src/dxf DXF_SOURCE)

# Gmsh library
AUX_SOURCE_DIRECTORY(src/gmsh GMSH_SOURCE)

# OCC library
AUX_SOURCE_DIRECTORY(src/occ OCC_SOURCE)

# VTK library
AUX_SOURCE_DIRECTORY(src/vtk VTK_SOURCE)


## SOURCES
# DXFToGLTF
set(DXFToGLTFSOURCE
  src/DXFToGLTF.cpp
  ${DXFLib_SOURCE}
  ${DXF_SOURCE}
  ${OCC_SOURCE}
  ${LOGGER_SOURCE}
)

# GmshToGLTF
set(GmshToGLTFSOURCE
  src/GmshToGLTF.cpp
  ${GMSH_SOURCE}
  ${OCC_SOURCE}
  ${LOGGER_SOURCE}
)

# StepToGLTF
set(StepToGLTFSOURCE
  src/StepToGLTF.cpp
  ${OCC_SOURCE}
  ${LOGGER_SOURCE}
)

# VTUToGLTF
set(VTUToGLTFSOURCE
  src/VTUToGLTF.cpp
  ${GMSH_SOURCE}
  ${VTK_SOURCE}
  ${OCC_SOURCE}
  ${LOGGER_SOURCE}
)

## Executables
# DXFToGLTF
add_executable(DXFToGLTF ${DXFToGLTFSOURCE})
target_link_libraries(DXFToGLTF ${OCC_LIBS})

# GmshToGLTF
add_executable(GmshToGLTF ${GmshToGLTFSOURCE})
target_link_libraries(GmshToGLTF ${OCC_LIBS})

# StepToGLTF
add_executable(StepToGLTF ${StepToGLTFSOURCE})
target_link_libraries(StepToGLTF ${OCC_LIBS})

# VTUToGLTF
add_executable(VTUToGLTF ${VTUToGLTFSOURCE})
target_link_libraries(VTUToGLTF ${VTK_LIBS} ${OCC_LIBS})

## Install
install(TARGETS DXFToGLTF StepToGLTF)

## Tests
include(CTest)
include(Catch)

set(LOGGER_TESTS
  test/logger/Logger.test.cpp
)

set(GMSH_TESTS
  test/gmsh/Gmsh.test.cpp
  test/gmsh/Tetrahedron.test.cpp
  test/gmsh/Triangle.test.cpp
  test/gmsh/Vertex.test.cpp
)

set(OCC_TESTS
  test/occ/GLTFWriter.test.cpp
  test/occ/StepReader.test.cpp
  test/occ/Triangulation.test.cpp
)

set(VTK_TESTS
  test/vtk/VTUReader.test.cpp
)

set(DXF_TESTS
  test/dxf/DXFConverter.test.cpp
)

set(SOURCE_TESTS
  test/main.cpp
  ${LOGGER_TESTS}
  ${GMSH_TESTS}
  ${OCC_TESTS}
  ${VTK_TESTS}
  ${DXF_TESTS}
  ${DXF_SOURCE}
  ${DXFLib_SOURCE}
  ${GMSH_SOURCE}
  ${LOGGER_SOURCE}
  ${OCC_SOURCE}
  ${VTK_SOURCE}
)

add_executable(tests ${SOURCE_TESTS})
target_link_libraries(tests ${OCC_LIBS})
target_link_libraries(tests ${VTK_LIBS})
target_link_libraries(tests Catch2::Catch2)
catch_discover_tests(tests)

## Coverage
option(COVERAGE "Enable coverage flags" OFF)
if (COVERAGE)
  SET(CMAKE_CXX_FLAGS "-O0 --coverage -W")
  
  add_custom_target(coverage
    COMMAND make clean
    COMMAND make -j"$(nproc)"
    COMMAND make test
    COMMAND ./DXFToGLTF || true
    COMMAND ./DXFToGLTF ../test/assets/not_existing.dxf pipe.glb|| true
    COMMAND ./DXFToGLTF ../test/assets/pipe.dxf pipe.glb || true
    COMMAND ./GmshToGLTF || true
    COMMAND ./GmshToGLTF ../test/assets/not_existing.msh not_existing.glb || true
    COMMAND ./GmshToGLTF ../test/assets/Cube.msh Cube.glb || true
    COMMAND ./StepToGLTF || true
    COMMAND ./StepToGLTF ../test/assets/not_existing.step not_existing.glb || true
    COMMAND ./StepToGLTF ../test/assets/cube.step cube.glb || true
    COMMAND ./VTUToGLTF || true
    COMMAND ./VTUToGLTF ../test/assets/not_existing.vtu not_existing || true
    COMMAND ./VTUToGLTF ../test/assets/Result.vtu Result || true
    COMMAND ./VTUToGLTF ../test/assets/Result2Pieces.vtu Result || true
    COMMAND lcov --directory . -c -o report0.info
    COMMAND lcov --remove report0.info '/usr/include/*' '/usr/local/include/*' '*/dxflib/*' -o report.info
    COMMAND genhtml -o ../coverage -t "Converters coverage" report.info
  )
endif()

