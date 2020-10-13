#include <catch2/catch.hpp>

#include <TDocStd_Document.hxx>
#include <TopoDS_Shape.hxx>
#include <vector>

#include "../../src/occ/StepReader.hpp"

TEST_CASE("StepReader") {
  SECTION("Constructor 1") { StepReader stepReader = StepReader(); }

  SECTION("Constructor 2") {
    std::string fileName = "fileName";
    StepReader stepReader = StepReader("fileName");
  }

  SECTION("read - no file") {
    std::string fileName = "fileName";
    StepReader stepReader = StepReader(fileName);

    bool res = stepReader.read();

    CHECK(!res);
  }

  SECTION("read - file") {
    std::string fileName = "./test/assets/cube.step";
    StepReader stepReader = StepReader(fileName);

    bool res = stepReader.read();

    CHECK(res);
  }

  SECTION("getShapes") {
    StepReader stepReader = StepReader();

    std::vector<TopoDS_Shape> shapes = stepReader.getShapes();
  }

  SECTION("getDocument") {
    StepReader stepReader = StepReader();

    Handle(TDocStd_Document) document = stepReader.getDocument();
  }
}