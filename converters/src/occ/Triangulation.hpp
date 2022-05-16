#ifndef _TRIANGULATION_
#define _TRIANGULATION_

#include <vector>

#include "MainDocument.hpp"

constexpr double meshQuality = 0.01;

class Triangulation {
private:
  MainDocument m_mainDocument;
  double m_minBb = 0;
  double m_maxBb = 0;
  // std::vector<float> m_vertices;
  // std::vector<float> m_normals;
  // std::vector<uint> m_indices;

  // Compute max bounding box
  void computeBb();
  // // Triangulate solid
  // void triangulateSolid(const TopoDS_Shape &);
  // // Triangulate face
  // void triangulateFace(const TopoDS_Shape &);
  // // Triangulate edge
  // void triangulateEdge(const TopoDS_Shape &);
  // Triangulate loop (solid & face)
  // void triangulateLoop(const TopoDS_Face &, const uint iDelta = 0);
  // Is valid
  // bool isValid(const gp_Pnt &, const gp_Pnt &, const gp_Pnt &) const;

public:
  // Constructor
  Triangulation();
  // Constructor
  explicit Triangulation(MainDocument &);

  // Triangulate
  void triangulate();
};

#endif //_TRIANGULATION_
