#ifndef _TRIANGULATION_
#define _TRIANGULATION_

#include <vector>

#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>

#define meshQuality 1.0

class Triangulation {
private:
  TopoDS_Shape shape;
  double minBb;
  double maxBb;
  std::vector<float> vertices;
  std::vector<float> normals;
  std::vector<uint> indices;

  // Compute max bounding box
  void computeBb();
  // Triangulate solid
  void triangulateSolid(TopoDS_Shape &shape);
  // Triangulate face
  void triangulateFace(TopoDS_Shape &shape);
  // Triangulate edge
  void triangulateEdge(TopoDS_Shape &shape);
  // Triangulate loop (solid & face)
  void triangulateLoop(TopoDS_Face &face, const uint iDelta=0);
  // Is valid
  bool isValid(const gp_Pnt&, const gp_Pnt&, const gp_Pnt&) const;
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
