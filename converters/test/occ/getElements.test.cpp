#include <catch2/catch.hpp>

#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Builder.hxx>

#include <vector>

#include "../../src/occ/getElements.hpp"

TEST_CASE("getElements") {
  SECTION("getSolids - empty") {
    TopoDS_Shape shape = TopoDS_Shape();
    std::vector<TopoDS_Shape> solids = getSolids(shape);
    CHECK(solids.size() == 0);
  }

  SECTION("getSolids - one") {
    TopoDS_Shape shape = TopoDS_Shape();
    BRepBuilderAPI_MakeSolid makeSolid = BRepBuilderAPI_MakeSolid();
    TopoDS_Solid solid = makeSolid.Solid();

    TopoDS_Builder::Add(shape, solid);
    std::vector<TopoDS_Shape> solids = getSolids(shape);
    CHECK(solids.size() == 1);
  }

  SECTION("getFaces - empty") {
    TopoDS_Shape shape = TopoDS_Shape();
    std::vector<TopoDS_Shape> faces = getFaces(shape);
    CHECK(faces.size() == 0);
  }

  SECTION("getFaces - one") {
    TopoDS_Shape shape = TopoDS_Shape();
    BRepBuilderAPI_MakeFace makeFace = BRepBuilderAPI_MakeFace();
    TopoDS_Face face = makeFace.Face();

    TopoDS_Builder::Add(shape, face);
    std::vector<TopoDS_Shape> faces = getFaces(shape);
    CHECK(faces.size() == 1);
  }

  SECTION("getEdges - empty") {
    TopoDS_Shape shape = TopoDS_Shape();
    std::vector<TopoDS_Shape> edges = getEdges(shape);
    CHECK(edges.size() == 0);
  }

  SECTION("getEdges - one") {
    TopoDS_Shape shape = TopoDS_Shape();
    BRepBuilderAPI_MakeFace makeEdge = BRepBuilderAPI_MakeEdge();
    TopoDS_Edge edge = makeEdge.Edge();

    TopoDS_Builder::Add(shape, edge);
    std::vector<TopoDS_Shape> edges = getEdges(shape);
    CHECK(edges.size() == 1);
  }
}
