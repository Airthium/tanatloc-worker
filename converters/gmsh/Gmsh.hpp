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
  uint numberOfVertices;
  // Number of triangles
  uint numberOfTriangles;
  // Number of triangle labels
  uint numberOfTriangleLabels;
  // Number of tetrahedra
  uint numberOfTetrahedra;
  // Number of tetrahedron labels
  uint numberOfTetrahedronLabels;

  // Vertices
  Vertex *vertices;
  // Triangles
  Triangle *triangles;
  // Triangle labels
  uint *triangleLabels;
  // Tetrahedra
  Tetrahedron *tetrahedra;
  // Tetrahedron labels
  uint *tetrahedronLabels;

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
