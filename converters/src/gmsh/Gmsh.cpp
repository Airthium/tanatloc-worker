#include "Gmsh.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

#include "../logger/Logger.hpp"

/**
 * Constructor
 */
Gmsh::Gmsh() {}

/**
 * Destructor
 */
Gmsh::~Gmsh() {
  if (this->m_vertices) {
    delete[] this->m_vertices;
    this->m_vertices = nullptr;
  }
  if (this->m_triangles) {
    delete[] this->m_triangles;
    this->m_triangles = nullptr;
  }
  if (this->m_triangleLabels) {
    delete[] this->m_triangleLabels;
    this->m_triangleLabels = nullptr;
  }
  if (this->m_tetrahedra) {
    delete[] this->m_tetrahedra;
    this->m_tetrahedra = nullptr;
  }
  if (this->m_tetrahedronLabels) {
    delete[] this->m_tetrahedronLabels;
    this->m_tetrahedronLabels = nullptr;
  }

  this->m_numberOfVertices = 0;
  this->m_numberOfTriangles = 0;
  this->m_numberOfTriangleLabels = 0;
  this->m_numberOfTetrahedra = 0;
  this->m_numberOfTetrahedronLabels = 0;
}

/**
 * Loader
 * @param {string} fileName File name
 * @returns {boolean} Loading status
 */
bool Gmsh::load(const std::string &fileName) {
  uint i;
  uint numberOfElements;
  std::string buffer;
  std::vector<Triangle> triangles;
  std::vector<Tetrahedron> tetrahedra;

  // Read file
  std::ifstream file(fileName, std::ios::in);
  if (!file) {
    Logger::ERROR("Unable to open " + fileName);
    return false;
  }

  // Vertices
  file >> buffer;
  while (buffer != "$Nodes" && !file.eof())
    file >> buffer;

  file >> this->m_numberOfVertices;
  if (!this->m_numberOfVertices) {
    std::cerr << "No vertices" << std::endl;
    return false;
  }

  this->m_vertices = new Vertex[this->m_numberOfVertices];

  for (i = 0; i < this->m_numberOfVertices; ++i) {
    double x;
    double y;
    double z;
    file >> buffer >> x >> y >> z;
    this->m_vertices[i].setCoordinates(x, y, z);
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
    uint j;
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

  this->m_numberOfTriangles = triangles.size();
  if (!this->m_numberOfTriangles) {
    std::cerr << "No triangles" << std::endl;
    return false;
  }
  this->m_triangles = new Triangle[this->m_numberOfTriangles];
  std::copy(triangles.begin(), triangles.end(), this->m_triangles);
  triangles.clear();

  this->m_numberOfTetrahedra = tetrahedra.size();
  this->m_tetrahedra = new Tetrahedron[this->m_numberOfTetrahedra];
  std::copy(tetrahedra.begin(), tetrahedra.end(), this->m_tetrahedra);
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

  for (i = 0; i < this->m_numberOfTriangles; ++i) {
    uint label = this->m_triangles[i].getLabel();
    it = find(triangleLabels.begin(), triangleLabels.end(), label);
    if (it == triangleLabels.end())
      triangleLabels.push_back(label);
  }

  for (i = 0; i < this->m_numberOfTetrahedra; ++i) {
    uint label = this->m_tetrahedra[i].getLabel();
    it = find(tetrahedronLabels.begin(), tetrahedronLabels.end(), label);
    if (it == tetrahedronLabels.end())
      tetrahedronLabels.push_back(label);
  }

  this->m_numberOfTriangleLabels = triangleLabels.size();
  this->m_triangleLabels = new uint[this->m_numberOfTriangleLabels];
  std::copy(triangleLabels.begin(), triangleLabels.end(),
            this->m_triangleLabels);
  triangleLabels.clear();

  this->m_numberOfTetrahedronLabels = tetrahedronLabels.size();
  this->m_tetrahedronLabels = new uint[this->m_numberOfTetrahedronLabels];
  std::copy(tetrahedronLabels.begin(), tetrahedronLabels.end(),
            this->m_tetrahedronLabels);
  tetrahedronLabels.clear();
}

/**
 * Get number of vertices
 * @returns Number of vertices
 */
uint Gmsh::getNumberOfVertices() const { return this->m_numberOfVertices; }

/**
 * Get number of triangles
 * @returns Number of triangles
 */
uint Gmsh::getNumberOfTriangles() const { return this->m_numberOfTriangles; }

/**
 * Get number of triangle labels
 * @returns Number of triangle labels
 */
uint Gmsh::getNumberOfTriangleLabels() const {
  return this->m_numberOfTriangleLabels;
}

/**
 * Get number of tetrahedra
 * @returns Number of tetrahedra
 */
uint Gmsh::getNumberOfTetrahedra() const { return this->m_numberOfTetrahedra; }

/**
 * Get number of tetrahedron labels
 * @returns Number of tetrahedron labels
 */
uint Gmsh::getNumberOfTetrahedronLabels() const {
  return this->m_numberOfTetrahedronLabels;
}

/**
 * Get tetrahedron label
 */
uint Gmsh::getTetrahedronLabel(const uint i) const {
  if (i < this->m_numberOfTetrahedronLabels)
    return this->m_tetrahedronLabels[i];
  return 0;
}

/**
 * Get triangle label
 */
uint Gmsh::getTriangleLabel(const uint i) const {
  if (i < this->m_numberOfTriangleLabels)
    return this->m_triangleLabels[i];
  return 0;
}

/**
 * Get volume vertices
 * @returns Vertices
 */
std::vector<double> *Gmsh::getVolumesVertices() const {
  uint i, j, k, l;

  if (!this->m_numberOfTetrahedronLabels)
    return nullptr;

  std::vector<double> *vertices =
      new std::vector<double>[this->m_numberOfTetrahedronLabels];
  for (i = 0; i < this->m_numberOfTetrahedronLabels; ++i) {
    for (j = 0; j < this->m_numberOfTetrahedra; ++j) {
      if (this->m_tetrahedra[j].getLabel() == this->m_tetrahedronLabels[i]) {
        uint *indices = this->m_tetrahedra[j].getIndices();
        for (k = 0; k < 4; ++k) {
          double *coordinates = this->m_vertices[indices[k]].getCoordinates();
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

  if (!this->m_numberOfTriangleLabels)
    return nullptr;

  std::vector<double> *vertices =
      new std::vector<double>[this->m_numberOfTriangleLabels];
  for (i = 0; i < this->m_numberOfTriangleLabels; ++i) {
    for (j = 0; j < this->m_numberOfTriangles; ++j) {
      if (this->m_triangles[j].getLabel() == this->m_triangleLabels[i]) {
        uint *indices = this->m_triangles[j].getIndices();
        for (k = 0; k < 3; ++k) {
          double *coordinates = this->m_vertices[indices[k]].getCoordinates();
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
