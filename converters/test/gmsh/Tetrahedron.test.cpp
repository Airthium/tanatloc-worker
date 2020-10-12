#include <catch2/catch.hpp>

#include "../../src/gmsh/Tetrahedron.hpp"

TEST_CASE("Tetrahedron") {
  SECTION("Constructor 1") {
    Tetrahedron tetrahedron = Tetrahedron();
    CHECK(tetrahedron.getIndex(0) == 0);
    CHECK(tetrahedron.getIndex(1) == 0);
    CHECK(tetrahedron.getIndex(2) == 0);
    CHECK(tetrahedron.getIndex(3) == 0);

    // Out of range
    CHECK(tetrahedron.getIndex(4) == 0);

    uint *indices = tetrahedron.getIndices();
    CHECK(indices[0] == 0);
    CHECK(indices[1] == 0);
    CHECK(indices[2] == 0);
    CHECK(indices[3] == 0);
    delete indices;

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
    CHECK(tetrahedron.getIndex(0) == 1);
    CHECK(tetrahedron.getIndex(1) == 2);
    CHECK(tetrahedron.getIndex(2) == 3);
    CHECK(tetrahedron.getIndex(3) == 4);

    uint *indices = tetrahedron.getIndices();
    CHECK(indices[0] == 1);
    CHECK(indices[1] == 2);
    CHECK(indices[2] == 3);
    CHECK(indices[3] == 4);
    delete indices;

    CHECK(tetrahedron.getLabel() == 5);
  }

  SECTION("setIndices") {
    uint index1 = 1;
    uint index2 = 2;
    uint index3 = 3;
    uint index4 = 4;
    Tetrahedron tetrahedron = Tetrahedron();
    tetrahedron.setIndices(index1, index2, index3, index4);
    CHECK(tetrahedron.getIndex(0) == 1);
    CHECK(tetrahedron.getIndex(1) == 2);
    CHECK(tetrahedron.getIndex(2) == 3);
    CHECK(tetrahedron.getIndex(3) == 4);

    uint *indices = tetrahedron.getIndices();
    CHECK(indices[0] == 1);
    CHECK(indices[1] == 2);
    CHECK(indices[2] == 3);
    CHECK(indices[3] == 4);
    delete indices;

    CHECK(tetrahedron.getLabel() == 0);
  }

  SECTION("setLabel") {
    uint label = 5;
    Tetrahedron tetrahedron = Tetrahedron();
    tetrahedron.setLabel(label);
    CHECK(tetrahedron.getIndex(0) == 0);
    CHECK(tetrahedron.getIndex(1) == 0);
    CHECK(tetrahedron.getIndex(2) == 0);
    CHECK(tetrahedron.getIndex(3) == 0);

    uint *indices = tetrahedron.getIndices();
    CHECK(indices[0] == 0);
    CHECK(indices[1] == 0);
    CHECK(indices[2] == 0);
    CHECK(indices[3] == 0);
    delete indices;

    CHECK(tetrahedron.getLabel() == 5);
  }
}
