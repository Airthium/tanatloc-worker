#include <catch2/catch.hpp>

#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Solid.hxx>

#include "../../src/occ/Triangulation.hpp"

TEST_CASE("Triangulation") {
  SECTION("Constructor 1") { Triangulation triangulation = Triangulation(); }

  SECTION("Constructor 2") {
    TopoDS_Shape shape = TopoDS_Shape();
    Triangulation triangulation = Triangulation(shape);

    double min;
    double max;
    triangulation.getBb(&min, &max);
    CHECK(min == 0);
    CHECK(max == 0);
  }

  SECTION("triangulate - no type") {
    TopoDS_Shape shape = TopoDS_Shape();
    Triangulation triangulation = Triangulation(shape);
    triangulation.triangulate();
  }

  SECTION("triangulate - solid") {
    TopoDS_Shape shape = TopoDS_Solid();
    Triangulation triangulation = Triangulation(shape);
    triangulation.triangulate();
  }

  SECTION("triangulate - face") {
    TopoDS_Shape shape = TopoDS_Face();
    Triangulation triangulation = Triangulation(shape);
    triangulation.triangulate();
  }

  SECTION("triangulate - edge") {
    TopoDS_Shape shape = TopoDS_Edge();
    Triangulation triangulation = Triangulation(shape);
    triangulation.triangulate();
  }
}