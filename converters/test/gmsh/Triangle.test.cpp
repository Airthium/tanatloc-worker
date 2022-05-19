#include <catch2/catch.hpp>

#include "../../src/gmsh/Triangle.hpp"

TEST_CASE("Triangle") {
  SECTION("Constructor 1") {
    Triangle triangle = Triangle();
    CHECK(triangle.getI1() == 0);
    CHECK(triangle.getI2() == 0);
    CHECK(triangle.getI3() == 0);

    std::vector<uint> indices = triangle.getIndices();
    CHECK(indices[0] == 0);
    CHECK(indices[1] == 0);
    CHECK(indices[2] == 0);

    CHECK(triangle.getLabel() == 0);
  }

  SECTION("Constructor 2") {
    uint index1 = 1;
    uint index2 = 2;
    uint index3 = 3;
    uint label = 4;
    Triangle triangle = Triangle(index1, index2, index3, label);
    CHECK(triangle.getI1() == index1);
    CHECK(triangle.getI2() == index2);
    CHECK(triangle.getI3() == index3);

    std::vector<uint> indices = triangle.getIndices();
    CHECK(indices[0] == index1);
    CHECK(indices[1] == index2);
    CHECK(indices[2] == index3);

    CHECK(triangle.getLabel() == label);
  }

  SECTION("setIndices") {
    uint index1 = 1;
    uint index2 = 2;
    uint index3 = 3;
    Triangle triangle = Triangle();
    triangle.setI1(index1);
    triangle.setI2(index2);
    triangle.setI3(index3);
    CHECK(triangle.getI1() == index1);
    CHECK(triangle.getI2() == index2);
    CHECK(triangle.getI3() == index3);

    std::vector<uint> indices = triangle.getIndices();
    CHECK(indices[0] == index1);
    CHECK(indices[1] == index2);
    CHECK(indices[2] == index3);

    CHECK(triangle.getLabel() == 0);
  }

  SECTION("Triangle setLabel") {
    uint label = 4;
    Triangle triangle = Triangle();
    triangle.setLabel(label);
    CHECK(triangle.getI1() == 0);
    CHECK(triangle.getI2() == 0);
    CHECK(triangle.getI3() == 0);

    std::vector<uint> indices = triangle.getIndices();
    CHECK(indices[0] == 0);
    CHECK(indices[1] == 0);
    CHECK(indices[2] == 0);

    CHECK(triangle.getLabel() == label);
  }
}
