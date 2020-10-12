#ifndef _GMSH_
#define _GMSH_

#include <string>
#include <vector>

#include "Tetrahedron.hpp"
#include "Triangle.hpp"
#include "Vertex.hpp"

typedef unsigned int uint;

/**
 * Gmsh class
 */
class Gmsh {
private:
  // Number of nodes
  uint m_numberOfVertices = 0;
  // Number of triangles
  uint m_numberOfTriangles = 0;
  // Number of triangle labels
  uint m_numberOfTriangleLabels = 0;
  // Number of tetrahedra
  uint m_numberOfTetrahedra = 0;
  // Number of tetrahedron labels
  uint m_numberOfTetrahedronLabels = 0;

  // Vertices
  Vertex *m_vertices = nullptr;
  // Triangles
  Triangle *m_triangles = nullptr;
  // Triangle labels
  uint *m_triangleLabels = nullptr;
  // Tetrahedra
  Tetrahedron *m_tetrahedra = nullptr;
  // Tetrahedron labels
  uint *m_tetrahedronLabels = nullptr;

public:
  // Constructor
  Gmsh();
  // Destructor
  ~Gmsh();

  // Loader
  bool load(const std::string &);

  // Compute number of different labels
  void computeLabels();

  // Get number of vertices
  uint getNumberOfVertices() const;
  // Get number of triangles
  uint getNumberOfTriangles() const;
  // Get number of triangle labels
  uint getNumberOfTriangleLabels() const;
  // Get number of tetrahedra
  uint getNumberOfTetrahedra() const;
  // Get number of tetrahedron labels
  uint getNumberOfTetrahedronLabels() const;

  // Get tetrahedron label
  uint getTetrahedronLabel(const uint) const;
  // Get triangle label
  uint getTriangleLabel(const uint) const;

  // Get volume vertices
  std::vector<double> *getVolumesVertices() const;
  // Get surface vertices
  std::vector<double> *getSurfacesVertices() const;
};

#endif
