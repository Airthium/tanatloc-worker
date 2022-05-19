#include <catch2/catch.hpp>

#include "../../src/occ/GLTFWriter.hpp"

TEST_CASE("GLTFWriter") {
  SECTION("Constructor") { GLTFWriter gltfWriter = GLTFWriter(); }
}