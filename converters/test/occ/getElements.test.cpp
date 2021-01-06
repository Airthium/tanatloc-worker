#include <catch2/catch.hpp>

#include <BRepPrimAPI_MakeBox.hxx>
#include <TopoDS_Shape.hxx>

#include <vector>

#include "../../src/occ/StepReader.hpp"
#include "../../src/occ/getElements.hpp"

TEST_CASE("getElements") {
  auto reader = StepReader("./test/assets/cube.step");
  reader.read();
  std::vector<TopoDS_Shape> shapes = reader.getShapes();
  Handle(TDocStd_Document) document = reader.getDocument();

  SECTION("getSolids - empty") {
    TopoDS_Shape shape = TopoDS_Shape();
    std::vector<TopoDS_Shape> solids = getSolids(shape);
    CHECK(solids.size() == 0);
  }

  SECTION("getSolids - box") {
    BRepPrimAPI_MakeBox box = BRepPrimAPI_MakeBox(1., 1., 1.);
    TopoDS_Shape solid = box.Solid();

    std::vector<TopoDS_Shape> solids = getSolids(solid);
    CHECK(solids.size() == 1);
  }

  SECTION("getSolids - document") {
    std::vector<std::pair<bool, Quantity_Color>> color =
        std::vector<std::pair<bool, Quantity_Color>>();
    std::vector<TopoDS_Shape> solids = getSolids(shapes[0], document, &color);
  }

  SECTION("getFaces - empty") {
    TopoDS_Shape shape = TopoDS_Shape();
    std::vector<TopoDS_Shape> faces = getFaces(shape);
    CHECK(faces.size() == 0);
  }

  SECTION("getFaces - box") {
    BRepPrimAPI_MakeBox box = BRepPrimAPI_MakeBox(1., 1., 1.);
    TopoDS_Shape solid = box.Solid();

    std::vector<TopoDS_Shape> faces = getFaces(solid);
    CHECK(faces.size() == 6);
  }

  SECTION("getFaces - document") {
    std::vector<std::pair<bool, Quantity_Color>> color =
        std::vector<std::pair<bool, Quantity_Color>>();
    std::vector<TopoDS_Shape> faces = getFaces(shapes[0], document, &color);
  }

  SECTION("getEdges - empty") {
    TopoDS_Shape shape = TopoDS_Shape();
    std::vector<TopoDS_Shape> edges = getEdges(shape);
    CHECK(edges.size() == 0);
  }

  SECTION("getEdges - box") {
    BRepPrimAPI_MakeBox box = BRepPrimAPI_MakeBox(1., 1., 1.);
    TopoDS_Shape solid = box.Solid();

    std::vector<TopoDS_Shape> edges = getEdges(solid);
    CHECK(edges.size() == 18);
  }

  SECTION("getEdges - document") {
    std::vector<std::pair<bool, Quantity_Color>> color =
        std::vector<std::pair<bool, Quantity_Color>>();
    std::vector<TopoDS_Shape> edges = getEdges(shapes[0], document, &color);
  }
}
