cmake_minimum_required(VERSION 3.0)

project(Tanatloc)

## LIBRARIES
# Catch2
find_package(Catch2 REQUIRED)

# OpenCASCADE
find_package(OpenCASCADE)
if (OpenCASCADE_FOUND)
  include_directories(${OpenCASCADE_INCLUDE_DIR})
  set(LIBS ${LIBS} ${OpenCASCADE_LIBRARY_DIR})
  set(OCC_LIBS ${OpenCASCADE_LIBRARIES})
endif (OpenCASCADE_FOUND)

# Logger library
AUX_SOURCE_DIRECTORY(src/logger LOGGER_SOURCE)

# DXF library
AUX_SOURCE_DIRECTORY(src/dxflib DXF_SOURCE)

# Gmsh library
AUX_SOURCE_DIRECTORY(src/gmsh GMSH_SOURCE)

# OCC library
AUX_SOURCE_DIRECTORY(src/occ OCC_SOURCE)

# ThreeJS library
AUX_SOURCE_DIRECTORY(src/threeJS THREEJS_SOURCE)

## SOURCES
# DXFToBRep
set(DXFToBRepSOURCE
  src/DXFToBRep.cpp
  src/DXFConverter.cpp
  ${DXF_SOURCE}
  ${LOGGER_SOURCE}
)

# BRepToThreeJS
set(BRepToThreeJSSOURCE
  src/BRepToThreeJS.cpp
  ${OCC_SOURCE}
  ${THREEJS_SOURCE}
  ${LOGGER_SOURCE}
)

# GmshToThreeJS
set(GmshToThreeJSSOURCE
  src/GmshToThreeJS.cpp
  ${GMSH_SOURCE}
  ${THREEJS_SOURCE}
  ${LOGGER_SOURCE}
)

# StepToThreeJS (TODO to remove)
set(StepToThreeJSSOURCE
  src/StepToThreeJS.cpp
  ${OCC_SOURCE}
  ${THREEJS_SOURCE}
  ${LOGGER_SOURCE}
)

# StepUnion (TODO to replace by BRepUnion)
set(StepUnionSOURCE
  src/StepUnion.cpp
  ${OCC_SOURCE}
  ${LOGGER_SOURCE}
)

## Executables
# DXFToBRep
add_executable(DXFToBRep ${DXFToBRepSOURCE})
target_link_libraries(DXFToBRep ${OCC_LIBS})

# BRepToThreeJS
add_executable(BRepToThreeJS ${BRepToThreeJSSOURCE})
target_link_libraries(BRepToThreeJS ${OCC_LIBS})

# GmshToThreeJS
add_executable(GmshToThreeJS ${GmshToThreeJSSOURCE})

# StepToThreeJS
add_executable(StepToThreeJS ${StepToThreeJSSOURCE})
target_link_libraries(StepToThreeJS ${OCC_LIBS})

# StepUnion
add_executable(StepUnion ${StepUnionSOURCE})
target_link_libraries(StepUnion ${OCC_LIBS})

## Tests
include(CTest)
include(ParseAndAddCatchTests)

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
  test/occ/getElements.test.cpp
  test/occ/StepReader.test.cpp
  test/occ/StepWriter.test.cpp
  test/occ/Triangulation.test.cpp
  test/occ/Union.test.cpp
)

set(THREEJS_TESTS
  test/threeJS/ThreeJS.test.cpp
)

set(MAIN_TESTS
  test/DXFConverter.test.cpp
  src/DXFConverter.cpp
)

set(SOURCE_TESTS
  test/main.cpp
  ${LOGGER_TESTS}
  ${LOGGER_SOURCE}
  ${GMSH_TESTS}
  ${GMSH_SOURCE}
  ${OCC_TESTS}
  ${OCC_SOURCE}
  ${THREEJS_TESTS}
  ${THREEJS_SOURCE}
  ${MAIN_TESTS}
  ${DXF_SOURCE}
)

add_executable(tests ${SOURCE_TESTS})
target_link_libraries(tests ${OCC_LIBS})
target_link_libraries(tests Catch2::Catch2)
ParseAndAddCatchTests(tests)

## Coverage
option(COVERAGE "Enable coverage flags" OFF)
if (COVERAGE)
  SET(CMAKE_CXX_FLAGS "-O0 --coverage")
endif()

add_custom_target(coverage
  COMMAND make clean
  COMMAND make
  COMMAND make test
  COMMAND ./BRepToThreeJS
  COMMAND ./GmshToThreeJS
  COMMAND ./StepToThreeJS
  COMMAND ./StepToThreeJS ./test/cube.step
  COMMAND ./StepToThreeJS ./test/cube.step ./cube
  COMMAND ./StepUnion
  COMMAND lcov --directory . -c -o report.info
  COMMAND genhtml -o ../coverage -t "Converters coverage" report.info
)