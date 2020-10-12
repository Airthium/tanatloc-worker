#include <catch2/catch.hpp>

#include <TopoDS_Shape.hxx>
#include <vector>

#include "../../src/occ/getElements.hpp"

TEST_CASE("getElements") {
  SECTION("getSolids") {
    TopoDS_Shape shape = TopoDS_Shape();
    std::vector<TopoDS_Shape> solids = getSolids(shape);
    CHECK(solids.size() == 0);
  }

  SECTION("getFaces") {
    TopoDS_Shape shape = TopoDS_Shape();
    std::vector<TopoDS_Shape> faces = getFaces(shape);
    CHECK(faces.size() == 0);
  }

  SECTION("getEdges") {
    TopoDS_Shape shape = TopoDS_Shape();
    std::vector<TopoDS_Shape> edges = getEdges(shape);
    CHECK(edges.size() == 0);
  }
}
