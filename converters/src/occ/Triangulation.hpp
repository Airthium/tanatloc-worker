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

  // Compute max bounding box
  void computeBb();

public:
  // Constructor
  Triangulation();
  // Constructor
  explicit Triangulation(const MainDocument &);

  // Triangulate
  void triangulate();

  // Triangulate face
  void triangulateFace(const TopoDS_Shape &) const;

  // Triangulate edge
  void triangulateEdge(const TopoDS_Shape &) const;
};

#endif //_TRIANGULATION_
