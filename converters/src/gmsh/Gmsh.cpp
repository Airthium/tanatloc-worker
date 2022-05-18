#include "Gmsh.hpp"

#include <algorithm>
#include <fstream>
#include <memory>

#include "../logger/Logger.hpp"

/**
 * Constructor
 */
Gmsh::Gmsh() = default;

/**
 * Loader
 * @param {string} fileName File name
 * @returns {boolean} Loading status
 */
bool Gmsh::load(const std::string &fileName) {
  uint i;
  uint numberOfVertices;
  uint numberOfElements;
  std::string buffer;
  std::ifstream file;

  // Read file
  file.open(fileName, std::ios::in);
  if (!file) {
    Logger::ERROR("Unable to open " + fileName);
    return false;
  }

  // Vertices
  file >> buffer;
  while (buffer != "$Nodes" && !file.eof())
    file >> buffer;

  file >> numberOfVertices;
  if (!numberOfVertices) {
    Logger::ERROR("No vertices");
    return false;
  }

  for (i = 0; i < numberOfVertices; ++i) {
    double x;
    double y;
    double z;
    file >> buffer >> x >> y >> z;
    Vertex vertex = Vertex(x, y, z);
    this->m_vertices.push_back(vertex);
  }

  // Elements
  file >> buffer;
  while (buffer != "$Elements" && !file.eof())
    file >> buffer;

  file >> numberOfElements;
  if (!numberOfElements) {
    Logger::ERROR("No elements");
    return false;
  }

  for (i = 0; i < numberOfElements; ++i) {
    uint type;
    uint numberOfLabels;
    uint label;
    file >> buffer >> type >> numberOfLabels;

    file >> label >> buffer;
    for (uint j = 2; j < numberOfLabels; ++j)
      file >> buffer;

    if (type == 2) { // 3-nodes triangle
      uint index1;
      uint index2;
      uint index3;
      file >> index1 >> index2 >> index3;
      Triangle triangle = Triangle(--index1, --index2, --index3, label);
      this->m_triangles.push_back(triangle);
    } else if (type == 4) { // 4-nodes tetrahedron
      uint index1;
      uint index2;
      uint index3;
      uint index4;
      file >> index1 >> index2 >> index3 >> index4;
      Tetrahedron tetrahedron =
          Tetrahedron(--index1, --index2, --index3, --index4, label);
      this->m_tetrahedra.push_back(tetrahedron);
    } else if (type == 15) { // 1-node point
      uint index1;
      file >> index1;
    } else if (type == 1) { // 2-node line
      uint index1;
      uint index2;
      file >> index1 >> index2;
    } else {
      Logger::ERROR("Unsupported type: " + type);
      return false;
    }
  }

  return true;
}

/**
 * Compute number of different labels
 */
void Gmsh::computeLabels() {

  this->m_triangleLabels.clear();
  this->m_tetrahedronLabels.clear();

  // Triangle labels
  std::for_each(this->m_triangles.begin(), this->m_triangles.end(),
                [this](const Triangle &T) {
                  const uint label = T.getLabel();
                  std::vector<uint>::iterator it =
                      std::find(this->m_triangleLabels.begin(),
                                this->m_triangleLabels.end(), label);
                  if (it == this->m_triangleLabels.end())
                    this->m_triangleLabels.push_back(label);
                });

  // Tetrahedra labels
  std::for_each(this->m_tetrahedra.begin(), this->m_tetrahedra.end(),
                [this](const Tetrahedron &T) {
                  const uint label = T.getLabel();
                  std::vector<uint>::iterator it =
                      std::find(this->m_tetrahedronLabels.begin(),
                                this->m_tetrahedronLabels.end(), label);
                  if (it == this->m_tetrahedronLabels.end())
                    this->m_tetrahedronLabels.push_back(label);
                });
}

/**
 * Get max dimension
 * @return Max
 */
double Gmsh::getMax() const {
  Vertex Min(this->m_vertices.at(0));
  Vertex Max(this->m_vertices.at(0));

  for (uint i = 1; i < this->m_vertices.size(); ++i) {
    double minX = Min.X();
    double minY = Min.Y();
    double minZ = Min.Z();

    double maxX = Max.X();
    double maxY = Max.Y();
    double maxZ = Max.Z();

    Vertex Current = this->m_vertices.at(i);
    double currentX = Current.X();
    double currentY = Current.Y();
    double currentZ = Current.Z();

    Min.setCoordinates(std::min(minX, currentX), std::min(minY, currentY),
                       std::min(minZ, currentZ));
    Max.setCoordinates(std::max(maxX, currentX), std::max(maxY, currentY),
                       std::max(maxZ, currentZ));
  }

  return std::max(Max.X() - Min.X(),
                  std::max(Max.Y() - Min.Y(), Max.Z() - Min.Z()));
}

/**
 * Get number of triangle labels
 * @returns Number of triangle labels
 */
uint Gmsh::getNumberOfTriangleLabels() const {
  return (uint)this->m_triangleLabels.size();
}

/**
 * Get number of tetrahedron labels
 * @returns Number of tetrahedron labels
 */
uint Gmsh::getNumberOfTetrahedronLabels() const {
  return (uint)this->m_tetrahedronLabels.size();
}

/**
 * Get tetrahedron label
 */
uint Gmsh::getTetrahedronLabel(const uint i) const {
  if (i < this->m_tetrahedronLabels.size())
    return this->m_tetrahedronLabels[i];
  return 0;
}

/**
 * Get triangle label
 */
uint Gmsh::getTriangleLabel(const uint i) const {
  if (i < this->m_triangleLabels.size())
    return this->m_triangleLabels[i];
  return 0;
}

/**
 * Copy vertices from tetrahedron to vector
 * @param {Tetrahedron} Tetrahedron
 * @param {vector} Vector
 */
void Gmsh::copyVertices(const Tetrahedron &T,
                        std::vector<Vertex> *vector) const {
  std::vector<uint> indices = T.getIndices();
  copy(indices, vector);
}

/**
 * Copy vertices from triangle to vector
 * @param {Triangle} Triangle
 * @param {vector} Vector
 */
void Gmsh::copyVertices(const Triangle &T, std::vector<Vertex> *vector) const {
  std::vector<uint> indices = T.getIndices();
  copy(indices, vector);
}

void Gmsh::copy(const std::vector<uint> indices,
                std::vector<Vertex> *vector) const {
  std::for_each(indices.begin(), indices.end(),
                [this, vector](const uint &index) {
                  Vertex vertex = this->m_vertices[index];
                  vector->push_back(vertex);
                });
}

/**
 * Get volume vertices
 * @returns Vertices
 */
std::vector<Vertex> *Gmsh::getVolumesVertices() const {
  if (!this->m_tetrahedronLabels.size())
    return nullptr;

  uint i = 0;
  auto vertices = new std::vector<Vertex>[this->m_tetrahedronLabels.size()];
  std::for_each(
      this->m_tetrahedronLabels.begin(), this->m_tetrahedronLabels.end(),
      [&i, this, vertices](const uint &label) {
        std::for_each(this->m_tetrahedra.begin(), this->m_tetrahedra.end(),
                      [i, this, vertices, label](const Tetrahedron &T) {
                        if (T.getLabel() == label) {
                          this->copyVertices(T, &vertices[i]);
                        }
                      });
        ++i;
      });

  return vertices;
}

/**
 * Get surface vertices
 * @returns Vertices
 */
std::vector<Vertex> *Gmsh::getSurfacesVertices() const {
  if (!this->m_triangleLabels.size())
    return nullptr;

  uint i = 0;
  auto *vertices = new std::vector<Vertex>[this->m_triangleLabels.size()];
  std::for_each(this->m_triangleLabels.begin(), this->m_triangleLabels.end(),
                [&i, this, vertices](const uint &label) {
                  std::for_each(this->m_triangles.begin(),
                                this->m_triangles.end(),
                                [i, this, vertices, label](const Triangle &T) {
                                  if (T.getLabel() == label) {
                                    this->copyVertices(T, &vertices[i]);
                                  }
                                });
                  ++i;
                });

  return vertices;
}
