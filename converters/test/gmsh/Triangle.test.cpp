#include <catch2/catch.hpp>

#include "../../src/gmsh/Triangle.hpp"

TEST_CASE("Triangle constructor 1") {
  INFO("Triangle::Triangle()")

  Triangle triangle = Triangle();
  CHECK(triangle.getIndex(0) == 0);
  CHECK(triangle.getIndex(1) == 0);
  CHECK(triangle.getIndex(2) == 0);

  // Out of range
  CHECK(triangle.getIndex(3) == 0);

  uint *indices = triangle.getIndices();
  CHECK(indices[0] == 0);
  CHECK(indices[1] == 0);
  CHECK(indices[2] == 0);
  delete indices;

  CHECK(triangle.getLabel() == 0);
}

TEST_CASE("Triangle constructor 2") {
  INFO("Triangle::Triangle(index1, index2, index3, label)");

  uint index1 = 1;
  uint index2 = 2;
  uint index3 = 3;
  uint label = 4;
  Triangle triangle = Triangle(index1, index2, index3, label);
  CHECK(triangle.getIndex(0) == 1);
  CHECK(triangle.getIndex(1) == 2);
  CHECK(triangle.getIndex(2) == 3);

  uint *indices = triangle.getIndices();
  CHECK(indices[0] == 1);
  CHECK(indices[1] == 2);
  CHECK(indices[2] == 3);
  delete indices;

  CHECK(triangle.getLabel() == 4);
}

TEST_CASE("Triangle setIndices") {
  INFO("Triangle::setIndices(index1, index2, index3)");

  uint index1 = 1;
  uint index2 = 2;
  uint index3 = 3;
  Triangle triangle = Triangle();
  triangle.setIndices(index1, index2, index3);
  CHECK(triangle.getIndex(0) == 1);
  CHECK(triangle.getIndex(1) == 2);
  CHECK(triangle.getIndex(2) == 3);

  uint *indices = triangle.getIndices();
  CHECK(indices[0] == 1);
  CHECK(indices[1] == 2);
  CHECK(indices[2] == 3);
  delete indices;

  CHECK(triangle.getLabel() == 0);
}

TEST_CASE("Triangle setLabel") {
  INFO("Triangle::setLabel(label)");

  uint label = 4;
  Triangle triangle = Triangle();
  triangle.setLabel(label);
  CHECK(triangle.getIndex(0) == 0);
  CHECK(triangle.getIndex(1) == 0);
  CHECK(triangle.getIndex(2) == 0);

  uint *indices = triangle.getIndices();
  CHECK(indices[0] == 0);
  CHECK(indices[1] == 0);
  CHECK(indices[2] == 0);
  delete indices;

  CHECK(triangle.getLabel() == 4);
}