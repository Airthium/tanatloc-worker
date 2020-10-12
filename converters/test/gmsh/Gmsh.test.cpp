#include <catch2/catch.hpp>

#include "../../src/gmsh/Gmsh.hpp"

TEST_CASE("Constructor") {
  Gmsh mesh = Gmsh();
  REQUIRE(mesh.getNumberOfVertices() == 0);
  REQUIRE(mesh.getNumberOfTriangles() == 0);
  REQUIRE(mesh.getNumberOfTriangleLabels() == 0);
  REQUIRE(mesh.getNumberOfTetrahedra() == 0);
  REQUIRE(mesh.getNumberOfTetrahedronLabels() == 0);

  REQUIRE(mesh.getVolumesVertices() == nullptr);
  REQUIRE(mesh.getSurfacesVertices() == nullptr);
}

TEST_CASE("load") {
  Gmsh mesh = Gmsh();

  bool res;

  res = mesh.load("");
  REQUIRE(res == false);
}