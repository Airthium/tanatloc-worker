#ifndef _GMSH_
#define _GMSH_

#include <string>
#include <vector>

#include "Tetrahedron.hpp"
#include "Triangle.hpp"
#include "Vertex.hpp"

using uint = unsigned int;

/**
 * Gmsh class
 */
class Gmsh {
private:
  // Vertices
  std::vector<Vertex> m_vertices = std::vector<Vertex>();
  // Triangles
  std::vector<Triangle> m_triangles = std::vector<Triangle>();
  // Triangle labels
  std::vector<uint> m_triangleLabels = std::vector<uint>();
  // Tetrahedra
  std::vector<Tetrahedron> m_tetrahedra = std::vector<Tetrahedron>();
  // Tetrahedron labels
  std::vector<uint> m_tetrahedronLabels = std::vector<uint>();

  // Copy vertices
  void copyVertices(const Tetrahedron &, std::vector<Vertex> *) const;
  void copyVertices(const Triangle &, std::vector<Vertex> *) const;
  void copy(const std::vector<uint> &, std::vector<Vertex> *) const;

public:
  // Constructor
  Gmsh();

  // Loader
  bool load(const std::string &);

  // Compute number of different labels
  void computeLabels();

  // Get max
  double getMax() const;

  // Get number of triangle labels
  uint getNumberOfTriangleLabels() const;

  // Get number of tetrahedron labels
  uint getNumberOfTetrahedronLabels() const;

  // Get tetrahedron label
  uint getTetrahedronLabel(const uint) const;

  // Get triangle label
  uint getTriangleLabel(const uint) const;

  // Get volume vertices
  std::vector<Vertex> *getVolumesVertices() const;

  // Get surface vertices
  std::vector<Vertex> *getSurfacesVertices() const;
};

#endif
