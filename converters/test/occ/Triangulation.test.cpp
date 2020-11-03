#include <catch2/catch.hpp>

#include <BRepPrimAPI_MakeBox.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Solid.hxx>

#include "../../src/occ/Triangulation.hpp"

TEST_CASE("Triangulation") {
  SECTION("Constructor 1") { Triangulation triangulation = Triangulation(); }

  SECTION("Constructor 2") {
    BRepPrimAPI_MakeBox box = BRepPrimAPI_MakeBox(1., 1., 1.);
    TopoDS_Solid solid = box.Solid();

    Triangulation triangulation = Triangulation(solid);

    double min;
    double max;
    triangulation.getBb(&min, &max);
    CHECK(min == 1.);
    CHECK(max == 1.);
  }

  SECTION("triangulate - solid") {
    BRepPrimAPI_MakeBox box = BRepPrimAPI_MakeBox(1., 1., 1.);
    TopoDS_Solid solid = box.Solid();

    Triangulation triangulation = Triangulation(solid);
    triangulation.triangulate();
  }

  SECTION("triangulate - face") {
    BRepPrimAPI_MakeBox box = BRepPrimAPI_MakeBox(1., 1., 1.);
    TopoDS_Face face = box.FrontFace();

    Triangulation triangulation = Triangulation(face);
    triangulation.triangulate();
  }
}