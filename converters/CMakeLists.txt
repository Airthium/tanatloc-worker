cmake_minimum_required(VERSION 3.0)

project(Tanatloc)

## Debug
option(DEBUG "Print debug informations" OFF)
if (DEBUG)
  add_definitions(-DDEBUG_MODE)
else()
  remove_definitions(-DDEBUG_MODE)
endif()

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

# ThreeJS library
AUX_SOURCE_DIRECTORY(src/threeJS THREEJS_SOURCE)

## SOURCES
# DXFToBRep
# set(DXFToBRepSOURCE
#   src/DXFToBRep.cpp
#   ${DXFLib_SOURCE}
#   ${DXF_SOURCE}
#   ${LOGGER_SOURCE}
# )

# DXFToGLTF
set(DXFToGLTFSOURCE
  src/DXFToGLTF.cpp
  ${DXFLib_SOURCE}
  ${DXF_SOURCE}
  ${OCC_SOURCE}
  ${LOGGER_SOURCE}
)

# # BRepToThreeJS
# set(BRepToThreeJSSOURCE
#   src/BRepToThreeJS.cpp
#   ${OCC_SOURCE}
#   ${THREEJS_SOURCE}
#   ${LOGGER_SOURCE}
# )

# # GmshToThreeJS
# set(GmshToThreeJSSOURCE
#   src/GmshToThreeJS.cpp
#   ${GMSH_SOURCE}
#   ${THREEJS_SOURCE}
#   ${LOGGER_SOURCE}
# )

# GmshToGLTF
set(GmshToGLTFSOURCE
  src/GmshToGLTF.cpp
  ${GMSH_SOURCE}
  ${OCC_SOURCE}
  ${LOGGER_SOURCE}
)

# # StepToThreeJS (TODO to remove)
# set(StepToThreeJSSOURCE
#   src/StepToThreeJS.cpp
#   ${OCC_SOURCE}
#   ${THREEJS_SOURCE}
#   ${LOGGER_SOURCE}
# )

# StepToGLTF (TODO to remove)
set(StepToGLTFSOURCE
  src/StepToGLTF.cpp
  ${OCC_SOURCE}
  ${LOGGER_SOURCE}
)

# # StepUnion (TODO to replace by BRepUnion)
# set(StepUnionSOURCE
#   src/StepUnion.cpp
#   ${OCC_SOURCE}
#   ${LOGGER_SOURCE}
# )

# VTUToGLTF
set(VTUToGLTFSOURCE
  src/VTUToGLTF.cpp
  ${GMSH_SOURCE}
  ${VTK_SOURCE}
  ${OCC_SOURCE}
  ${LOGGER_SOURCE}
)

## Executables
# DXFToBRep
# add_executable(DXFToBRep ${DXFToBRepSOURCE})
# target_link_libraries(DXFToBRep ${OCC_LIBS})

# DXFToGLTF
add_executable(DXFToGLTF ${DXFToGLTFSOURCE})
target_link_libraries(DXFToGLTF ${OCC_LIBS})

# # BRepToThreeJS
# add_executable(BRepToThreeJS ${BRepToThreeJSSOURCE})
# target_link_libraries(BRepToThreeJS ${OCC_LIBS})

# GmshToGLTF
add_executable(GmshToGLTF ${GmshToGLTFSOURCE})
target_link_libraries(GmshToGLTF ${OCC_LIBS})

# # StepToThreeJS
# add_executable(StepToThreeJS ${StepToThreeJSSOURCE})
# target_link_libraries(StepToThreeJS ${OCC_LIBS})

# StepToGLTF
add_executable(StepToGLTF ${StepToGLTFSOURCE})
target_link_libraries(StepToGLTF ${OCC_LIBS})

# # StepUnion
# add_executable(StepUnion ${StepUnionSOURCE})
# target_link_libraries(StepUnion ${OCC_LIBS})

# VTUToGLTF
add_executable(VTUToGLTF ${VTUToGLTFSOURCE})
target_link_libraries(VTUToGLTF ${VTK_LIBS} ${OCC_LIBS})

## Install
install(TARGETS DXFToGLTF StepToGLTF)

# ## Tests
# include(CTest)
# include(Catch)

# set(LOGGER_TESTS
#   test/logger/Logger.test.cpp
# )

# set(GMSH_TESTS
#   test/gmsh/Gmsh.test.cpp
#   test/gmsh/Tetrahedron.test.cpp
#   test/gmsh/Triangle.test.cpp
#   test/gmsh/Vertex.test.cpp
# )

# set(OCC_TESTS
#   test/occ/getElements.test.cpp
#   test/occ/StepReader.test.cpp
#   test/occ/StepWriter.test.cpp
#   test/occ/Triangulation.test.cpp
#   test/occ/Union.test.cpp
# )

# set(THREEJS_TESTS
#   test/threeJS/ThreeJS.test.cpp
# )

# set(VTK_TESTS
#   test/vtk/VTUReader.test.cpp
# )

# set(MAIN_TESTS
#   test/dxf/DXFConverter.test.cpp
# )

# set(SOURCE_TESTS
#   test/main.cpp
#   ${LOGGER_TESTS}
#   ${GMSH_TESTS}
#   ${OCC_TESTS}
#   ${VTK_TESTS}
#   ${MAIN_TESTS}
#   ${THREEJS_TESTS}
#   ${LOGGER_SOURCE}
#   ${GMSH_SOURCE}
#   ${OCC_SOURCE}
#   ${THREEJS_SOURCE}
#   ${VTK_SOURCE}
#   ${DXFLib_SOURCE}
#   ${DXF_SOURCE}
# )

# add_executable(tests ${SOURCE_TESTS})
# target_link_libraries(tests ${OCC_LIBS})
# target_link_libraries(tests ${VTK_LIBS})
# target_link_libraries(tests Catch2::Catch2)
# catch_discover_tests(tests)

# ## Coverage
# option(COVERAGE "Enable coverage flags" OFF)
# if (COVERAGE)
#   SET(CMAKE_CXX_FLAGS "-O0 --coverage -W")
# endif()

# add_custom_target(coverage
#   COMMAND make clean
#   COMMAND make
#   COMMAND make test
#   COMMAND ./DXFToBRep || true
#   COMMAND ./DXFToBRep ./test/assets/not_existing.dxf pipe.brep|| true
#   COMMAND ./DXFToBRep ./test/assets/pipe.dxf pipe.brep || true
#   COMMAND ./BRepToThreeJS || true
#   COMMAND ./BRepToThreeJS ./test/assets/not_existing.brep || true
#   COMMAND ./BRepToThreeJS ./test/assets/pipe.brep || true
#   COMMAND ./BRepToThreeJS ./test/assets/cube.brep || true
#   COMMAND ./BRepToThreeJS ./test/assets/pipe.brep pipe || true
#   COMMAND ./GmshToThreeJS || true
#   COMMAND ./GmshToThreeJS ./test/assets/not_existing.msh || true
#   COMMAND ./GmshToThreeJS ./test/assets/Cube.msh || true
#   COMMAND ./GmshToThreeJS ./test/assets/Cube.msh ./cube || true
#   COMMAND ./StepToThreeJS || true
#   COMMAND ./StepToThreeJS ./test/assets/not_existing.step || true
#   COMMAND ./StepToThreeJS ./test/assets/cube.step || true
#   COMMAND ./StepToThreeJS ./test/assets/cube.step ./cube || true
#   COMMAND ./StepUnion || true
#   COMMAND ./StepUnion ./test/assets/not_existing.step ./cube.step || true
#   COMMAND ./StepUnion ./test/assets/not_existing.step ./not_existing/cube.step || true
#   COMMAND ./StepUnion ./test/assets/cube.step ./cube.step || true
#   COMMAND ./VTUToThreeJS || true
#   COMMAND ./VTUToThreeJS ./test/assets/not_existing.vtu || true
#   COMMAND ./VTUToThreeJS ./test/assets/Result.vtu || true
#   COMMAND ./VTUToThreeJS ./test/assets/Result.vtu Result || true
#   COMMAND ./VTUToThreeJS ./test/assets/Result_multi.vtu Result || true
#   COMMAND lcov --directory . -c -o report.info
#   COMMAND genhtml -o ../coverage -t "Converters coverage" report.info
# )