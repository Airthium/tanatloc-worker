#include <catch2/catch.hpp>

#include <BRepBuilderAPI_MakeSolid.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Solid.hxx>

#include "../../src/occ/StepWriter.hpp"

TEST_CASE("StepWriter") {
  SECTION("Constructor 1") { StepWriter stepWriter = StepWriter(); }

  SECTION("Constructor 2") {
    std::string fileName = "fileName";
    TopoDS_Shape shape = TopoDS_Shape();
    StepWriter stepWriter = StepWriter(fileName, shape);
  }

  SECTION("write") {
    std::string fileName = "fileName";
    TopoDS_Shape shape = TopoDS_Shape();
    StepWriter stepWriter = StepWriter(fileName, shape);

    bool res = stepWriter.write();
    CHECK(!res);

    BRepBuilderAPI_MakeSolid makeSolid = BRepBuilderAPI_MakeSolid();
    TopoDS_Solid solid = makeSolid.Solid();
    StepWriter solidWriter = StepWriter(fileName, solid);

    res = solidWriter.write();
    CHECK(res);

    // StepWriter failWriter = StepWriter("/", solid);

    // res = failWriter.write();
    // CHECK(!res);
  }
}