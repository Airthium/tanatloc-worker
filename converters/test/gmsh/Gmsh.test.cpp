#include <catch2/catch.hpp>

#include "../../src/gmsh/Gmsh.hpp"

TEST_CASE("Constructor") {
  Gmsh mesh = Gmsh();
  REQUIRE(mesh.getNumberOfVertices() == 0);
}