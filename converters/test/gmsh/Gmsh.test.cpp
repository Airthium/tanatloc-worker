#include <catch2/catch.hpp>

#include "../../src/gmsh/Gmsh.hpp"

TEST_CASE("Gmsh") {

  SECTION("Constructor") {
    Gmsh mesh = Gmsh();
    CHECK(mesh.getNumberOfVertices() == 0);
    CHECK(mesh.getNumberOfTriangles() == 0);
    CHECK(mesh.getNumberOfTriangleLabels() == 0);
    CHECK(mesh.getNumberOfTetrahedra() == 0);
    CHECK(mesh.getNumberOfTetrahedronLabels() == 0);

    CHECK(mesh.getTetrahedronLabel(0) == 0);
    CHECK(mesh.getTriangleLabel(0) == 0);

    CHECK(mesh.getVolumesVertices() == nullptr);
    CHECK(mesh.getSurfacesVertices() == nullptr);
  }

  SECTION("load - no file") {
    Gmsh mesh = Gmsh();

    bool res = mesh.load("");
    CHECK(!res);
  }

  SECTION("load - wrong file") {
    Gmsh mesh = Gmsh();

    bool res = mesh.load("./test/assets/Cube.wrong.msh");
    CHECK(res == false);
  }

  SECTION("load - correct file") {
    Gmsh mesh = Gmsh();

    bool res = mesh.load("./test/assets/Cube.msh");
    CHECK(res == true);
  }

  SECTION("load - no vertices") {
    Gmsh mesh = Gmsh();

    bool res = mesh.load("./test/assets/Cube.no_vertices.msh");
    CHECK(res == false);
  }

  SECTION("load - no elements") {
    Gmsh mesh = Gmsh();

    bool res = mesh.load("./test/assets/Cube.no_elements.msh");
    CHECK(res == false);
  }

  SECTION("load - no triangles") {
    Gmsh mesh = Gmsh();

    bool res = mesh.load("./test/assets/Cube.no_triangles.msh");
    CHECK(res == false);
  }

  SECTION("load - 2D") {
    Gmsh mesh = Gmsh();

    bool res = mesh.load("./test/assets/Square.msh");
    CHECK(res == true);
  }

  SECTION("load - 1D") {
    Gmsh mesh = Gmsh();

    bool res = mesh.load("./test/assets/Point.msh");
    CHECK(res == true);
  }

  SECTION("computeLabels") {
    Gmsh mesh = Gmsh();

    mesh.load("./test/assets/Cube.msh");
    mesh.computeLabels();

    CHECK(mesh.getTriangleLabel(0) == 1);
    CHECK(mesh.getTetrahedronLabel(0) == 1);

    // Out of bound
    CHECK(mesh.getTriangleLabel(10) == 0);
    CHECK(mesh.getTetrahedronLabel(10) == 0);
  }

  SECTION("getVolumesVertices") {
    Gmsh mesh = Gmsh();

    mesh.load("./test/assets/Cube.msh");
    mesh.computeLabels();
    std::vector<double> *vertices = mesh.getVolumesVertices();
    CHECK(vertices->size() > 0);
  }

  SECTION("getSurfacesVertices") {
    Gmsh mesh = Gmsh();

    mesh.load("./test/assets/Cube.msh");
    mesh.computeLabels();
    std::vector<double> *vertices = mesh.getSurfacesVertices();
    CHECK(vertices->size() > 0);
  }
}
