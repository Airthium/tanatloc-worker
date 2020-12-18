#include <catch2/catch.hpp>

#include "../../src/vtk/VTUReader.hpp"

TEST_CASE("VTUReader") {
  SECTION("Constructor 1") { VTUReader reader = VTUReader(); }

  SECTION("Constructor 2") { VTUReader reader = VTUReader("filename"); }

  SECTION("getArrays") {
    VTUReader reader = VTUReader();

    std::vector<RData> arrays = reader.getArrays();
    CHECK(arrays.size() == 0);
  }

  SECTION("read") {
    VTUReader reader = VTUReader("./test/assets/Result.vtu");
    reader.read();

    std::vector<RData> arrays = reader.getArrays();
    CHECK(arrays.size() == 1);
  }

  SECTION("read 2 pieces") {
    VTUReader reader = VTUReader("./test/assets/Result2Pieces.vtu");
    reader.read();

    std::vector<RData> arrays = reader.getArrays();
    CHECK(arrays.size() == 0);
  }
}