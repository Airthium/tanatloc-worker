#include "Triangulation.hpp"

#include <BRepBndLib.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_Tool.hxx>
#include <Bnd_Box.hxx>
#include <Poly_Connect.hxx>
#include <ShapeAnalysis_Edge.hxx>
#include <StdPrs_ToolTriangulatedShape.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>

#include "../logger/Logger.hpp"

/**
 * Constructor
 */
Triangulation::Triangulation() = default;

/**
 * Constructor
 * @param shape Shape
 */
Triangulation::Triangulation(TopoDS_Shape shape) : m_shape(shape) {
  this->computeBb();
}

/**
 * Compute max bounding box dimension
 * @returns Max bounding box dimension
 */
void Triangulation::computeBb() {
  Bnd_Box boundingBox;
  double xMin;
  double yMin;
  double zMin;
  double xMax;
  double yMax;
  double zMax;

  BRepBndLib::Add(this->m_shape, boundingBox);
  boundingBox.Get(xMin, yMin, zMin, xMax, yMax, zMax);

  double xDim = std::abs(xMax - xMin);
  double yDim = std::abs(yMax - yMin);
  double zDim = std::abs(zMax - zMin);

  this->m_minBb = std::min(xDim, std::min(yDim, zDim));
  this->m_maxBb = std::max(xDim, std::max(yDim, zDim));
}

/**
 * Triangulate
 */
void Triangulation::triangulate() {
  TopAbs_ShapeEnum type = this->m_shape.ShapeType();
  if (type == TopAbs_SOLID)
    this->triangulateSolid(this->m_shape);
  else if (type == TopAbs_FACE)
    this->triangulateFace(this->m_shape);
  else if (type == TopAbs_EDGE)
    this->triangulateEdge(this->m_shape);
}

/**
 * Triangulate solid [private]
 * @param shape Shape
 */
void Triangulation::triangulateSolid(const TopoDS_Shape &shape) {
  TopExp_Explorer explorer;
  BRepMesh_IncrementalMesh mesh(shape, this->m_maxBb * meshQuality);
  for (explorer.Init(shape, TopAbs_FACE); explorer.More(); explorer.Next()) {
    TopoDS_Face face = TopoDS::Face(explorer.Current());
    this->triangulateLoop(face, (uint)this->m_vertices.size() / 3);
  }
}

/**
 * Triangulate face [private]
 * @param shape Shape
 */
void Triangulation::triangulateFace(const TopoDS_Shape &shape) {
  BRepMesh_IncrementalMesh mesh(shape, this->m_maxBb * meshQuality);
  this->triangulateLoop(TopoDS::Face(shape));
}

/**
 * Triangulate edge [private]
 * @param shape Shape
 */
void Triangulation::triangulateEdge(const TopoDS_Shape &shape) {
  gp_Pnt p;

  BRepMesh_IncrementalMesh mesh(shape, this->m_maxBb * 2.e-2 * meshQuality,
                                false, 0.5 * meshQuality, false);
  TopLoc_Location location;
  Handle(Poly_Polygon3D) polygon =
      BRep_Tool::Polygon3D(TopoDS::Edge(shape), location);

  const TColgp_Array1OfPnt &nodes = polygon->Nodes();
  for (uint i = nodes.Lower(); i <= (uint)nodes.Upper(); ++i) {
    p = nodes(i).Transformed(location.Transformation());
    this->m_vertices.push_back(static_cast<float>(p.X()));
    this->m_vertices.push_back(static_cast<float>(p.Y()));
    this->m_vertices.push_back(static_cast<float>(p.Z()));
  }
}

/**
 * Triangulate loop [private]
 * @param face Face
 */
void Triangulation::triangulateLoop(const TopoDS_Face &face,
                                    const uint iDelta) {
  uint i;
  gp_Pnt p;
  gp_Dir d;

  TopLoc_Location location;
  Handle(Poly_Triangulation) triangulation =
      BRep_Tool::Triangulation(face, location);

  if (triangulation.IsNull()) {
    Logger::ERROR("Null triangulation");
    return;
  }
  Poly_Connect pc(triangulation);

  // vertices
  const TColgp_Array1OfPnt &nodes = triangulation->Nodes();
  for (i = nodes.Lower(); i <= (uint)nodes.Upper(); ++i) {
    p = nodes(i).Transformed(location.Transformation());
    this->m_vertices.push_back(static_cast<float>(p.X()));
    this->m_vertices.push_back(static_cast<float>(p.Y()));
    this->m_vertices.push_back(static_cast<float>(p.Z()));
  }

  // normal
  TColgp_Array1OfDir normals(nodes.Lower(), nodes.Upper());
  StdPrs_ToolTriangulatedShape::Normal(face, pc, normals);
  for (i = normals.Lower(); i <= (uint)normals.Upper(); ++i) {
    d = normals(i).Transformed(location.Transformation());
    this->m_normals.push_back(static_cast<float>(d.X()));
    this->m_normals.push_back(static_cast<float>(d.Y()));
    this->m_normals.push_back(static_cast<float>(d.Z()));
  }

  // indices
  int n1;
  int n2;
  int n3;
  TopAbs_Orientation orient = face.Orientation();
  const Poly_Array1OfTriangle &triangles = triangulation->Triangles();
  for (i = 1; i <= (uint)triangulation->NbTriangles(); ++i) {
    triangles(i).Get(n1, n2, n3);
    if (orient != TopAbs_FORWARD) {
      int tmp = n1;
      n1 = n2;
      n2 = tmp;
    }
    if (this->isValid(nodes(n1), nodes(n2), nodes(n3))) {
      this->m_indices.push_back(--n1 + iDelta);
      this->m_indices.push_back(--n2 + iDelta);
      this->m_indices.push_back(--n3 + iDelta);
    }
  }
}

/**
 * Get Bb
 */
void Triangulation::getBb(double *min, double *max) const {
  *min = this->m_minBb;
  *max = this->m_maxBb;
}

/**
 * Get vertices
 * @returns Vertices
 */
std::vector<float> Triangulation::getVertices() const {
  return this->m_vertices;
}

/**
 * Get normals
 * @returns normals
 */
std::vector<float> Triangulation::getNormals() const { return this->m_normals; }

/**
 * Get indices
 * @returns indices
 */
std::vector<uint> Triangulation::getIndices() const { return this->m_indices; }

/**
 * Is valid
 * @param p1 Point 1
 * @param p2 Point 2
 * @param p3 Point 3
 * @returns Is valid
 */
bool Triangulation::isValid(const gp_Pnt &p1, const gp_Pnt &p2,
                            const gp_Pnt &p3) const {
  gp_Vec v1(p1, p2);
  gp_Vec v2(p2, p3);
  gp_Vec v3(p3, p1);

  if ((v1.SquareMagnitude() > 1.e-10) && (v2.SquareMagnitude() > 1.e-10) &&
      (v3.SquareMagnitude() > 1.e-10)) {
    v1.Cross(v2);
    if (v1.SquareMagnitude() > 1.e-10)
      return true;
    else
      return false;
  }
  return false;
}
