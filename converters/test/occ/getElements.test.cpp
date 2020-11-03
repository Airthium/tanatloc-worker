#include <catch2/catch.hpp>

#include <BRepPrimAPI_MakeBox.hxx>
#include <Quantity_Color.hxx>
#include <TDocStd_Document.hxx>
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

  // SECTION("getSolids - cube") {
  //   TopoDS_Builder builder = TopoDS_Builder();
  //   BRepPrimAPI_MakeBox box = BRepPrimAPI_MakeBox(1., 1., 1.);
  //   TopoDS_Shape solid = box.Solid();

  //   Handle(TDocStd_Document) document = Handle(TDocStd_Document)();
  //   std::vector<std::pair<bool, Quantity_Color>> color;

  //   std::vector<TopoDS_Shape> solids = getSolids(solid, document, &color);
  //   CHECK(solids.size() == 1);
  // }

  SECTION("getFaces - empty") {
    TopoDS_Shape shape = TopoDS_Shape();
    std::vector<TopoDS_Shape> faces = getFaces(shape);
    CHECK(faces.size() == 0);
  }

  SECTION("getFaces - box") {
    TopoDS_Builder builder = TopoDS_Builder();
    BRepPrimAPI_MakeBox box = BRepPrimAPI_MakeBox(1., 1., 1.);
    TopoDS_Shape face = box.FrontFace();

    std::vector<TopoDS_Shape> faces = getFaces(face);
    CHECK(faces.size() == 1);
  }

  SECTION("getEdges - empty") {
    TopoDS_Shape shape = TopoDS_Shape();
    std::vector<TopoDS_Shape> edges = getEdges(shape);
    CHECK(edges.size() == 0);
  }

  // SECTION("getEdges - one") {
  //   TopoDS_Builder builder = TopoDS_Builder();
  //   TopoDS_Shape shape = TopoDS_Shape();
  //   BRepBuilderAPI_MakeEdge makeEdge = BRepBuilderAPI_MakeEdge();
  //   TopoDS_Edge edge = makeEdge.Edge();

  //   builder.Add(shape, edge);
  //   std::vector<TopoDS_Shape> edges = getEdges(shape);
  //   CHECK(edges.size() == 1);
  // }
}
