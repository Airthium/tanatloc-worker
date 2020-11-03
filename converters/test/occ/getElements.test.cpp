#include <catch2/catch.hpp>

#include <BRepPrimAPI_MakeBox.hxx>
#include <TopoDS_Builder.hxx>
#include <TopoDS_Shape.hxx>

#include <vector>

#include "../../src/occ/getElements.hpp"

TEST_CASE("getElements") {
  SECTION("getSolids - empty") {
    TopoDS_Shape shape = TopoDS_Shape();
    std::vector<TopoDS_Shape> solids = getSolids(shape);
    CHECK(solids.size() == 0);
  }

  SECTION("getSolids - box") {
    TopoDS_Builder builder = TopoDS_Builder();
    BRepPrimAPI_MakeBox box = BRepPrimAPI_MakeBox(1., 1., 1.);
    TopoDS_Shape solid = box.Solid();

    std::vector<TopoDS_Shape> solids = getSolids(solid);
    CHECK(solids.size() == 1);
  }

  SECTION("getFaces - empty") {
    TopoDS_Shape shape = TopoDS_Shape();
    std::vector<TopoDS_Shape> faces = getFaces(shape);
    CHECK(faces.size() == 0);
  }

  SECTION("getFaces - box") {
    TopoDS_Builder builder = TopoDS_Builder();
    BRepPrimAPI_MakeBox box = BRepPrimAPI_MakeBox(1., 1., 1.);
    TopoDS_Shape solid = box.Solid();

    std::vector<TopoDS_Shape> faces = getFaces(solid);
    CHECK(faces.size() == 6);
  }

  SECTION("getEdges - empty") {
    TopoDS_Shape shape = TopoDS_Shape();
    std::vector<TopoDS_Shape> edges = getEdges(shape);
    CHECK(edges.size() == 0);
  }

  SECTION("getEdges - box") {
    TopoDS_Builder builder = TopoDS_Builder();
    BRepPrimAPI_MakeBox box = BRepPrimAPI_MakeBox(1., 1., 1.);
    TopoDS_Shape solid = box.Solid();

    std::vector<TopoDS_Shape> edges = getEdges(solid);
    CHECK(edges.size() == 18);
  }
}
