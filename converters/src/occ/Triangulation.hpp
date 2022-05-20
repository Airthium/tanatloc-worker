#ifndef _TRIANGULATION_
#define _TRIANGULATION_

#include "MainDocument.hpp"

constexpr double meshQuality = 1.e-3;

class Triangulation {
private:
  MainDocument m_mainDocument;
  double m_minBb = 0;
  double m_maxBb = 0;

  // Compute max bounding box
  void computeBb();

  // Triangulate face
  void triangulateFace(const TopoDS_Shape &) const;

  // Triangulate edge
  void triangulateEdge(const TopoDS_Shape &) const;

public:
  // Constructor
  Triangulation();
  // Constructor
  explicit Triangulation(const MainDocument &);

  // Triangulate
  void triangulate();
};

#endif //_TRIANGULATION_
