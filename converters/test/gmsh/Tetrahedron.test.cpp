#include <catch2/catch.hpp>

#include "../../src/gmsh/Tetrahedron.hpp"

TEST_CASE("Tetrahedron") {
  SECTION("Constructor 1") {
    Tetrahedron tetrahedron = Tetrahedron();
    CHECK(tetrahedron.getI1() == 0);
    CHECK(tetrahedron.getI2() == 0);
    CHECK(tetrahedron.getI3() == 0);
    CHECK(tetrahedron.getI4() == 0);

    std::vector<uint> indices = tetrahedron.getIndices();
    CHECK(indices[0] == 0);
    CHECK(indices[1] == 0);
    CHECK(indices[2] == 0);
    CHECK(indices[3] == 0);

    CHECK(tetrahedron.getLabel() == 0);
  }

  SECTION("Constructor 2") {
    uint index1 = 1;
    uint index2 = 2;
    uint index3 = 3;
    uint index4 = 4;
    uint label = 5;
    Tetrahedron tetrahedron =
        Tetrahedron(index1, index2, index3, index4, label);
    CHECK(tetrahedron.getI1() == index1);
    CHECK(tetrahedron.getI2() == index2);
    CHECK(tetrahedron.getI3() == index3);
    CHECK(tetrahedron.getI4() == index4);

    std::vector<uint> indices = tetrahedron.getIndices();
    CHECK(indices[0] == index1);
    CHECK(indices[1] == index2);
    CHECK(indices[2] == index3);
    CHECK(indices[3] == index4);

    CHECK(tetrahedron.getLabel() == label);
  }

  SECTION("setIndices") {
    uint index1 = 1;
    uint index2 = 2;
    uint index3 = 3;
    uint index4 = 4;
    Tetrahedron tetrahedron = Tetrahedron();
    tetrahedron.setI1(index1);
    tetrahedron.setI2(index2);
    tetrahedron.setI3(index3);
    tetrahedron.setI4(index4);
    CHECK(tetrahedron.getI1() == index1);
    CHECK(tetrahedron.getI2() == index2);
    CHECK(tetrahedron.getI3() == index3);
    CHECK(tetrahedron.getI4() == index4);

    std::vector<uint> indices = tetrahedron.getIndices();
    CHECK(indices[0] == index1);
    CHECK(indices[1] == index2);
    CHECK(indices[2] == index3);
    CHECK(indices[3] == index4);

    CHECK(tetrahedron.getLabel() == 0);
  }

  SECTION("setLabel") {
    uint label = 5;
    Tetrahedron tetrahedron = Tetrahedron();
    tetrahedron.setLabel(label);
    CHECK(tetrahedron.getI1() == 0);
    CHECK(tetrahedron.getI2() == 0);
    CHECK(tetrahedron.getI3() == 0);
    CHECK(tetrahedron.getI4() == 0);

    std::vector<uint> indices = tetrahedron.getIndices();
    CHECK(indices[0] == 0);
    CHECK(indices[1] == 0);
    CHECK(indices[2] == 0);
    CHECK(indices[3] == 0);

    CHECK(tetrahedron.getLabel() == label);
  }
}
