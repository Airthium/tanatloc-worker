#include <catch2/catch.hpp>

#include <TopoDS_Shape.hxx>

#include "../../src/occ/StepWriter.hpp"

TEST_CASE("StepWriter") {
  SECTION("Constructor 1") { StepWriter stepWriter = StepWriter(); }

  SECTION("Constructor 2") {
    std::string fileName = "fileName";
    TopoDS_Shape shape = TopoDS_Shape();
    StepWriter stepWriter = StepWriter(fileName, shape);
  }
}