#include <catch2/catch.hpp>

#include "../../src/threeJS/ThreeJS.hpp"

TEST_CASE("ThreeJS") {
  SECTION("Constructor 1") { ThreeJS threejs = ThreeJS(); }

  SECTION("Constructor 2") {
    uint size = 3;
    float *vertices = new float[size];
    for (uint i = 0; i < size; ++i)
      vertices[i] = (float)i;

    ThreeJS threejs = ThreeJS(vertices, size);
  }

  SECTION("Constructor 3") {
    uint size = 3;
    double *vertices = new double[size];
    for (uint i = 0; i < size; ++i)
      vertices[i] = (double)i;

    ThreeJS threejs = ThreeJS(vertices, size);
  }

  SECTION("Constructor 4") {
    uint vsize = 3;
    float *vertices = new float[vsize];
    for (uint i = 0; i < vsize; ++i)
      vertices[i] = (float)i;

    uint nsize = 3;
    float *normals = new float[nsize];
    for (uint i = 0; i < nsize; ++i)
      normals[i] = (float)i;

    ThreeJS threejs = ThreeJS(vertices, vsize, normals, nsize);
  }

  SECTION("Constructor 5") {
    uint vsize = 3;
    float *vertices = new float[vsize];
    for (uint i = 0; i < vsize; ++i)
      vertices[i] = (float)i;

    uint nsize = 3;
    float *normals = new float[nsize];
    for (uint i = 0; i < nsize; ++i)
      normals[i] = (float)i;

    uint isize = 3;
    uint *indices = new uint[isize];
    for (uint i = 0; i < nsize; ++i)
      indices[i] = i;

    ThreeJS threejs = ThreeJS(vertices, vsize, normals, nsize, indices, isize);
  }

  SECTION("setLabel") {
    ThreeJS threejs = ThreeJS();
    threejs.setLabel(0);
  }

  SECTION("setVertices") {
    ThreeJS threejs = ThreeJS();

    uint vsize = 3;
    float *vertices = new float[vsize];
    for (uint i = 0; i < vsize; ++i)
      vertices[i] = (float)i;
    threejs.setVertices(vertices, vsize);

    threejs.setVertices(vertices, vsize);
  }

  SECTION("setNormals") {
    ThreeJS threejs = ThreeJS();

    uint nsize = 3;
    float *normals = new float[nsize];
    for (uint i = 0; i < nsize; ++i)
      normals[i] = (float)i;
    threejs.setNormals(normals, nsize);

    threejs.setNormals(normals, nsize);
  }

  SECTION("setIndices") {
    ThreeJS threejs = ThreeJS();

    uint isize = 3;
    uint *indices = new uint[isize];
    for (uint i = 0; i < isize; ++i)
      indices[i] = i;
    threejs.setIndices(indices, isize);

    threejs.setIndices(indices, isize);
  }

  SECTION("setColors") {
    ThreeJS threejs = ThreeJS();

    uint csize = 3;
    float **colors = new float *[csize];
    for (uint i = 0; i < csize; ++i)
      colors[i] = new float[3];
    threejs.setColors(colors, csize);

    threejs.setColors(colors, csize);
  }
}
