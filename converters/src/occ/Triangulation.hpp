#ifndef _TRIANGULATION_
#define _TRIANGULATION_

#include <vector>

#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>

constexpr double meshQuality = 0.01;

class Triangulation {
private:
  TopoDS_Shape m_shape;
  double m_minBb = 0;
  double m_maxBb = 0;
  std::vector<float> m_vertices;
  std::vector<float> m_normals;
  std::vector<uint> m_indices;

  // Compute max bounding box
  void computeBb();
  // Triangulate solid
  void triangulateSolid(const TopoDS_Shape &shape);
  // Triangulate face
  void triangulateFace(const TopoDS_Shape &shape);
  // Triangulate edge
  void triangulateEdge(const TopoDS_Shape &shape);
  // Triangulate loop (solid & face)
  void triangulateLoop(const TopoDS_Face &face, const uint iDelta = 0);
  // Is valid
  bool isValid(const gp_Pnt &, const gp_Pnt &, const gp_Pnt &) const;

public:
  // Constructor
  Triangulation();
  // Constructor
  explicit Triangulation(TopoDS_Shape shape);
  // Destructor
  ~Triangulation();

  // Triangulate
  void triangulate();

  // Get Bb
  void getBb(double *, double *) const;
  // Get vertices
  std::vector<float> getVertices() const;
  // Get normals
  std::vector<float> getNormals() const;
  // Get indices
  std::vector<uint> getIndices() const;
};

#endif //_TRIANGULATION_
