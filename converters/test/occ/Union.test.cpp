#include <catch2/catch.hpp>

#include <BRepPrimAPI_MakeBox.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Solid.hxx>
#include <vector>

#include "../../src/occ/Union.hpp"

TEST_CASE("Union") {
  SECTION("Constructor 1") { Union u = Union(); }

  SECTION("Constructor 2") {
    uint i;
    uint nObjects = 2;
    uint nTools = 3;

    double tolerance = 1.;

    auto objects = std::vector<TopoDS_Shape>();
    auto tools = std::vector<TopoDS_Shape>();

    for (i = 0; i < nObjects; ++i)
      objects.push_back(TopoDS_Shape());

    for (i = 0; i < nTools; ++i)
      tools.push_back(TopoDS_Shape());

    Union u = Union(objects, tools, tolerance);
  }

  SECTION("setObjects") {
    uint nObjects = 2;
    auto objects = std::vector<TopoDS_Shape>();
    for (uint i = 0; i < nObjects; ++i)
      objects.push_back(TopoDS_Shape());

    Union u = Union();

    u.setObjects(objects);

    u.setObjects(objects);
  }

  SECTION("setObjects") {
    uint nTools = 3;
    auto tools = std::vector<TopoDS_Shape>();
    for (uint i = 0; i < nTools; ++i)
      tools.push_back(TopoDS_Shape());

    Union u = Union();

    u.setTools(tools);

    u.setTools(tools);
  }

  SECTION("setTolerance") {
    double tolerance = 1.;

    Union u = Union();

    u.setTolerance(tolerance);
  }

  SECTION("Compute") {
    Union u = Union();
    u.compute();

    u.setTolerance(1.);
    u.compute();

    uint nObjects = 1;
    uint nTools = 1;

    BRepPrimAPI_MakeBox box = BRepPrimAPI_MakeBox(1., 1., 1.);
    TopoDS_Solid solid = box.Solid();
    auto objects = std::vector<TopoDS_Shape>();
    objects.push_back(solid);
    auto tools = std::vector<TopoDS_Shape>();
    tools.push_back(solid);
    double tolerance = 1.;

    Union u2 = Union(objects, tools, tolerance);
    u2.compute();
  }
}