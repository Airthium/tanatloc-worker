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
  uint numberOfVertices = 0;
  // Number of triangles
  uint numberOfTriangles = 0;
  // Number of triangle labels
  uint numberOfTriangleLabels = 0;
  // Number of tetrahedra
  uint numberOfTetrahedra = 0;
  // Number of tetrahedron labels
  uint numberOfTetrahedronLabels = 0;

  // Vertices
  Vertex *vertices = nullptr;
  // Triangles
  Triangle *triangles = nullptr;
  // Triangle labels
  uint *triangleLabels = nullptr;
  // Tetrahedra
  Tetrahedron *tetrahedra = nullptr;
  // Tetrahedron labels
  uint *tetrahedronLabels = nullptr;

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
