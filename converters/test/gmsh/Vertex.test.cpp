#include <catch2/catch.hpp>

#include "../../src/gmsh/Vertex.hpp"

TEST_CASE("Vertex") {
  SECTION("Construtor 1") {
    Vertex vertex = Vertex();
    CHECK(vertex.getCoordinate(0) == 0);
    CHECK(vertex.getCoordinate(1) == 0);
    CHECK(vertex.getCoordinate(2) == 0);

    // Out of range
    CHECK(vertex.getCoordinate(3) == 0);

    double *coordinates = vertex.getCoordinates();
    CHECK(coordinates[0] == 0);
    CHECK(coordinates[1] == 0);
    CHECK(coordinates[2] == 0);
    delete coordinates;
  }

  SECTION("Construtor 2") {
    double x = 1.;
    double y = 2.;
    double z = 3;
    Vertex vertex = Vertex(x, y, z);
    CHECK(vertex.getCoordinate(0) == x);
    CHECK(vertex.getCoordinate(1) == y);
    CHECK(vertex.getCoordinate(2) == z);

    double *coordinates = vertex.getCoordinates();
    CHECK(coordinates[0] == x);
    CHECK(coordinates[1] == y);
    CHECK(coordinates[2] == z);
    delete coordinates;
  }

  SECTION("Vertex setCoordinates") {
    double x = 1.;
    double y = 2.;
    double z = 3.;
    Vertex vertex = Vertex();
    vertex.setCoordinates(x, y, z);
    CHECK(vertex.getCoordinate(0) == 1.);
    CHECK(vertex.getCoordinate(1) == 2.);
    CHECK(vertex.getCoordinate(2) == 3.);

    double *coordinates = vertex.getCoordinates();
    CHECK(coordinates[0] == 1.);
    CHECK(coordinates[1] == 2.);
    CHECK(coordinates[2] == 3.);
    delete coordinates;
  }
}
