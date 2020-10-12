#include "Gmsh.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

/**
 * Constructor
 */
Gmsh::Gmsh() {}

/**
 * Destructor
 */
Gmsh::~Gmsh() {
  if (this->vertices) {
    delete[] this->vertices;
    this->vertices = nullptr;
  }
  if (this->triangles) {
    delete[] this->triangles;
    this->triangles = nullptr;
  }
  if (this->triangleLabels) {
    delete[] this->triangleLabels;
    this->triangleLabels = nullptr;
  }
  if (this->tetrahedra) {
    delete[] this->tetrahedra;
    this->tetrahedra = nullptr;
  }
  if (this->tetrahedronLabels) {
    delete[] this->tetrahedronLabels;
    this->tetrahedronLabels = nullptr;
  }

  this->numberOfVertices = 0;
  this->numberOfTriangles = 0;
  this->numberOfTriangleLabels = 0;
  this->numberOfTetrahedra = 0;
  this->numberOfTetrahedronLabels = 0;
}

/**
 * Loader
 * @param {string} fileName File name
 * @returns {boolean} Loading status
 */
bool Gmsh::load(const std::string &fileName) {
  uint i;
  uint j;
  uint numberOfElements;
  std::string buffer;
  std::vector<Triangle> triangles;
  std::vector<Tetrahedron> tetrahedra;

  // Read file
  std::ifstream file(fileName, std::ios::in);
  if (!file) {
    std::cerr << "Unable to open " << fileName << std::endl;
    return false;
  }

  // Vertices
  file >> buffer;
  while (buffer != "$Nodes" && !file.eof())
    file >> buffer;

  file >> this->numberOfVertices;
  if (!this->numberOfVertices) {
    std::cerr << "No vertices" << std::endl;
    return false;
  }

  this->vertices = new Vertex[this->numberOfVertices];

  for (i = 0; i < this->numberOfVertices; ++i) {
    double x;
    double y;
    double z;
    file >> buffer >> x >> y >> z;
    this->vertices[i].setCoordinates(x, y, z);
  }

  // Elements
  file >> buffer;
  while (buffer != "$Elements" && !file.eof())
    file >> buffer;

  file >> numberOfElements;
  if (!numberOfElements) {
    std::cerr << "No elements" << std::endl;
    return false;
  }

  for (i = 0; i < numberOfElements; ++i) {
    uint type;
    uint numberOfLabels;
    uint label;
    file >> buffer >> type >> numberOfLabels;

    file >> buffer >> label;
    for (j = 2; j < numberOfLabels; ++j)
      file >> buffer;

    if (type == 2) { // 3-nodes triangle
      uint index1;
      uint index2;
      uint index3;
      file >> index1 >> index2 >> index3;
      Triangle triangle = Triangle(--index1, --index2, --index3, label);
      triangles.push_back(triangle);
    } else if (type == 4) { // 4-nodes tetrahedron
      uint index1;
      uint index2;
      uint index3;
      uint index4;
      file >> index1 >> index2 >> index3 >> index4;
      Tetrahedron tetrahedron =
          Tetrahedron(--index1, --index2, --index3, --index4, label);
      tetrahedra.push_back(tetrahedron);
    } else if (type == 15) { // 1-node point
      uint index1;
      file >> index1;
    } else if (type == 1) { // 2-node line
      uint index1;
      uint index2;
      file >> index1 >> index2;
    } else {
      std::cerr << "Unsupported type: " << type << std::endl;
      return false;
    }
  }

  this->numberOfTriangles = triangles.size();
  if (!this->numberOfTriangles) {
    std::cerr << "No triangles" << std::endl;
    return false;
  }
  this->triangles = new Triangle[this->numberOfTriangles];
  std::copy(triangles.begin(), triangles.end(), this->triangles);
  triangles.clear();

  this->numberOfTetrahedra = tetrahedra.size();
  this->tetrahedra = new Tetrahedron[this->numberOfTetrahedra];
  std::copy(tetrahedra.begin(), tetrahedra.end(), this->tetrahedra);
  tetrahedra.clear();

  return true;
}

/**
 * Compute number of different labels
 */
void Gmsh::computeLabels() {
  uint i;
  std::vector<uint> triangleLabels;
  std::vector<uint> tetrahedronLabels;
  std::vector<uint>::iterator it;

  for (i = 0; i < this->numberOfTriangles; ++i) {
    uint label = this->triangles[i].getLabel();
    it = find(triangleLabels.begin(), triangleLabels.end(), label);
    if (it == triangleLabels.end())
      triangleLabels.push_back(label);
  }

  for (i = 0; i < this->numberOfTetrahedra; ++i) {
    uint label = this->tetrahedra[i].getLabel();
    it = find(tetrahedronLabels.begin(), tetrahedronLabels.end(), label);
    if (it == tetrahedronLabels.end())
      tetrahedronLabels.push_back(label);
  }

  this->numberOfTriangleLabels = triangleLabels.size();
  this->triangleLabels = new uint[this->numberOfTriangleLabels];
  std::copy(triangleLabels.begin(), triangleLabels.end(), this->triangleLabels);
  triangleLabels.clear();

  this->numberOfTetrahedronLabels = tetrahedronLabels.size();
  this->tetrahedronLabels = new uint[this->numberOfTetrahedronLabels];
  std::copy(tetrahedronLabels.begin(), tetrahedronLabels.end(),
            this->tetrahedronLabels);
  tetrahedronLabels.clear();
}

/**
 * Get number of vertices
 * @returns Number of vertices
 */
uint Gmsh::getNumberOfVertices() const { return this->numberOfVertices; }

/**
 * Get number of triangles
 * @returns Number of triangles
 */
uint Gmsh::getNumberOfTriangles() const { return this->numberOfTriangles; }

/**
 * Get number of triangle labels
 * @returns Number of triangle labels
 */
uint Gmsh::getNumberOfTriangleLabels() const {
  return this->numberOfTriangleLabels;
}

/**
 * Get number of tetrahedra
 * @returns Number of tetrahedra
 */
uint Gmsh::getNumberOfTetrahedra() const { return this->numberOfTetrahedra; }

/**
 * Get number of tetrahedron labels
 * @returns Number of tetrahedron labels
 */
uint Gmsh::getNumberOfTetrahedronLabels() const {
  return this->numberOfTetrahedronLabels;
}

/**
 * Get tetrahedron label
 */
uint Gmsh::getTetrahedronLabel(const uint i) const {
  if (i < this->numberOfTetrahedronLabels)
    return this->tetrahedronLabels[i];
  return 0;
}

/**
 * Get triangle label
 */
uint Gmsh::getTriangleLabel(const uint i) const {
  if (i < this->numberOfTriangleLabels)
    return this->triangleLabels[i];
  return 0;
}

/**
 * Get volume vertices
 * @returns Vertices
 */
std::vector<double> *Gmsh::getVolumesVertices() const {
  uint i, j, k, l;

  if (!this->numberOfTetrahedronLabels)
    return nullptr;

  std::vector<double> *vertices =
      new std::vector<double>[this->numberOfTetrahedronLabels];
  for (i = 0; i < this->numberOfTetrahedronLabels; ++i) {
    for (j = 0; j < this->numberOfTetrahedra; ++j) {
      if (this->tetrahedra[j].getLabel() == this->tetrahedronLabels[i]) {
        uint *indices = this->tetrahedra[j].getIndices();
        for (k = 0; k < 4; ++k) {
          double *coordinates = this->vertices[indices[k]].getCoordinates();
          for (l = 0; l < 3; ++l) {
            vertices[i].push_back(coordinates[l]);
          }
          delete[] coordinates;
        }
        delete[] indices;
      }
    }
  }

  return vertices;
}

/**
 * Get surface vertices
 * @returns Vertices
 */
std::vector<double> *Gmsh::getSurfacesVertices() const {
  uint i, j, k, l;

  if (!this->numberOfTriangleLabels)
    return nullptr;

  std::vector<double> *vertices =
      new std::vector<double>[this->numberOfTriangleLabels];
  for (i = 0; i < this->numberOfTriangleLabels; ++i) {
    for (j = 0; j < this->numberOfTriangles; ++j) {
      if (this->triangles[j].getLabel() == this->triangleLabels[i]) {
        uint *indices = this->triangles[j].getIndices();
        for (k = 0; k < 3; ++k) {
          double *coordinates = this->vertices[indices[k]].getCoordinates();
          for (l = 0; l < 3; ++l) {
            vertices[i].push_back(coordinates[l]);
          }
          delete[] coordinates;
        }
        delete[] indices;
      }
    }
  }

  return vertices;
}
