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

  SECTION("convert") {
    DXFConverter dxfConverter = DXFConverter();
    dxfConverter.setInput("./test/assets/pipe.dxf");
    dxfConverter.convert();
  }

  SECTION("write") {
    DXFConverter dxfConverter = DXFConverter();
    dxfConverter.setOutput("pipe.brep");
    dxfConverter.write();
  }
}