#include <catch2/catch.hpp>

#include <BRepPrimAPI_MakeBox.hxx>
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

  SECTION("write - empty") {
    std::string fileName = "fileName.step";
    TopoDS_Shape shape = TopoDS_Shape();
    StepWriter stepWriter = StepWriter(fileName, shape);

    bool res = stepWriter.write();
    CHECK(!res);
  }

  SECTION("write - with solid") {
    std::string fileName = "fileName.step";
    BRepPrimAPI_MakeBox box = BRepPrimAPI_MakeBox(1., 1., 1.);
    TopoDS_Solid solid = box.Solid();
    StepWriter solidWriter = StepWriter(fileName, solid);

    bool res = solidWriter.write();
    CHECK(res);
  }

  SECTION("write - error") {
    std::string fileName = "/not_authorized.step";
    BRepPrimAPI_MakeBox box = BRepPrimAPI_MakeBox(1., 1., 1.);
    TopoDS_Solid solid = box.Solid();
    StepWriter solidWriter = StepWriter(fileName, solid);

    bool res = solidWriter.write();
    CHECK(!res);
  }
}