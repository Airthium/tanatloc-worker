#include <catch2/catch.hpp>

#include "../../src/gmsh/Gmsh.hpp"

TEST_CASE("Gmsh") {

  SECTION("Constructor") {
    Gmsh mesh = Gmsh();
    REQUIRE(mesh.getNumberOfVertices() == 0);
    REQUIRE(mesh.getNumberOfTriangles() == 0);
    REQUIRE(mesh.getNumberOfTriangleLabels() == 0);
    REQUIRE(mesh.getNumberOfTetrahedra() == 0);
    REQUIRE(mesh.getNumberOfTetrahedronLabels() == 0);

    REQUIRE(mesh.getTetrahedronLabel(0) == 0);
    REQUIRE(mesh.getTriangleLabel(0) == 0);

    REQUIRE(mesh.getVolumesVertices() == nullptr);
    REQUIRE(mesh.getSurfacesVertices() == nullptr);
  }

  SECTION("load - no file") {
    SECTION("Empty") {
      INFO("Gmsh::load()");
      Gmsh mesh = Gmsh();

      bool res = mesh.load("");
      REQUIRE(!res);
    }
  }

  SECTION("load - empty file") {
    INFO("Gmsh::load(file)");
    Gmsh mesh = Gmsh();

    bool res = mesh.load("./test/assets/Cube.msh");
    REQUIRE(res == true);
  }
}
// TODO
