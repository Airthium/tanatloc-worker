#include <catch2/catch.hpp>

#include "../src/DXFConverter.hpp"

TEST_CASE("DXFConverter") {
  SECTION("Constructor") { DXFConverter dxfConverter = DXFConverter(); }

  SECTION("setInput") {
    DXFConverter dxfConverter = DXFConverter();

    dxfConverter.setInput("input");
  }

  SECTION("setOutput") {
    DXFConverter dxfConverter = DXFConverter();

    dxfConverter.setOutput("output");
  }
}