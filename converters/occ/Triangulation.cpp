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

/**
 * Constructor
 */
Triangulation::Triangulation()
    : shape(), minBb(0.), maxBb(0.), vertices(), normals(), indices() {}

/**
 * Constructor
 * @param shape Shape
 */
Triangulation::Triangulation(TopoDS_Shape shape)
    : shape(shape), minBb(0.), maxBb(0.), vertices(), normals(), indices() {
  this->computeBb();
}

/**
 * Destructor
 */
Triangulation::~Triangulation() { this->maxBb = 0; }

/**
 * Compute max bounding box dimension
 * @returns Max bounding box dimension
 */
void Triangulation::computeBb() {
  Bnd_Box boundingBox;
  double xMin, yMin, zMin, xMax, yMax, zMax;

  BRepBndLib::Add(this->shape, boundingBox);
  boundingBox.Get(xMin, yMin, zMin, xMax, yMax, zMax);

  double xDim = std::abs((double)xMax - (double)xMin);
  double yDim = std::abs((double)yMax - (double)yMin);
  double zDim = std::abs((double)zMax - (double)zMin);

  this->minBb = std::min(xDim, std::min(yDim, zDim));
  this->maxBb = std::max(xDim, std::max(yDim, zDim));
}

/**
 * Triangulate
 */
void Triangulation::triangulate() {
  TopAbs_ShapeEnum type = this->shape.ShapeType();
  if (type == TopAbs_SOLID)
    this->triangulateSolid(this->shape);
  else if (type == TopAbs_FACE)
    this->triangulateFace(this->shape);
  else if (type == TopAbs_EDGE)
    this->triangulateEdge(this->shape);
}

/**
 * Triangulate solid [private]
 * @param shape Shape
 */
void Triangulation::triangulateSolid(TopoDS_Shape &shape) {
  TopExp_Explorer explorer;
  BRepMesh_IncrementalMesh(shape, this->maxBb * meshQuality);
  for (explorer.Init(shape, TopAbs_FACE); explorer.More(); explorer.Next()) {
    TopoDS_Face face = TopoDS::Face(explorer.Current());
    this->triangulateLoop(face, this->vertices.size() / 3);
  }
}

/**
 * Triangulate face [private]
 * @param shape Shape
 */
void Triangulation::triangulateFace(TopoDS_Shape &shape) {
  BRepMesh_IncrementalMesh(shape, this->maxBb * meshQuality);
  this->triangulateLoop(TopoDS::Face(shape));
}

/**
 * Triangulate edge [private]
 * @param shape Shape
 */
void Triangulation::triangulateEdge(TopoDS_Shape &shape) {
  // TopoDS_Edge edge = TopoDS::Edge(shape);
  // ShapeAnalysis_Edge analyser;
  // TopoDS_Vertex start = analyser.FirstVertex(edge);
  // TopoDS_Vertex end = analyser.LastVertex(edge);
  //
  // gp_Pnt startPoint = BRep_Tool::Pnt(start);
  // gp_Pnt endPoint = BRep_Tool::Pnt(end);
  //
  // this->vertices.push_back(static_cast<float>(startPoint.X()));
  // this->vertices.push_back(static_cast<float>(startPoint.Y()));
  // this->vertices.push_back(static_cast<float>(startPoint.Z()));
  //
  // this->vertices.push_back(static_cast<float>((startPoint.X() + endPoint.X())
  // / 2.)); this->vertices.push_back(static_cast<float>((startPoint.Y() +
  // endPoint.Y()) / 2.));
  // this->vertices.push_back(static_cast<float>((startPoint.Z() + endPoint.Z())
  // / 2.));
  //
  // this->vertices.push_back(static_cast<float>(endPoint.X()));
  // this->vertices.push_back(static_cast<float>(endPoint.Y()));
  // this->vertices.push_back(static_cast<float>(endPoint.Z()));
  uint i;
  gp_Pnt p;

  BRepMesh_IncrementalMesh(shape, this->maxBb * 2.e-2 * meshQuality, false,
                           0.5 * meshQuality, false);
  TopLoc_Location location;
  Handle(Poly_Polygon3D) polygon =
      BRep_Tool::Polygon3D(TopoDS::Edge(shape), location);

  const TColgp_Array1OfPnt &nodes = polygon->Nodes();
  for (i = nodes.Lower(); i <= nodes.Upper(); ++i) {
    p = nodes(i).Transformed(location.Transformation());
    this->vertices.push_back(static_cast<float>(p.X()));
    this->vertices.push_back(static_cast<float>(p.Y()));
    this->vertices.push_back(static_cast<float>(p.Z()));
  }
}

/**
 * Triangulate loop [private]
 * @param face Face
 */
void Triangulation::triangulateLoop(TopoDS_Face &face, const uint iDelta) {
  uint i;
  gp_Pnt p;
  gp_Dir d;
  StdPrs_ToolTriangulatedShape SST;

  TopLoc_Location location;
  Handle(Poly_Triangulation) triangulation =
      BRep_Tool::Triangulation(face, location);

  if (triangulation.IsNull()) {
    std::cerr << "Null triangulation" << std::endl;
    return;
  }
  Poly_Connect pc(triangulation);

  // vertices
  const TColgp_Array1OfPnt &nodes = triangulation->Nodes();
  for (i = nodes.Lower(); i <= nodes.Upper(); ++i) {
    p = nodes(i).Transformed(location.Transformation());
    this->vertices.push_back(static_cast<float>(p.X()));
    this->vertices.push_back(static_cast<float>(p.Y()));
    this->vertices.push_back(static_cast<float>(p.Z()));
  }

  // normal
  TColgp_Array1OfDir normals(nodes.Lower(), nodes.Upper());
  SST.Normal(face, pc, normals);
  for (i = normals.Lower(); i <= normals.Upper(); ++i) {
    d = normals(i).Transformed(location.Transformation());
    this->normals.push_back(static_cast<float>(d.X()));
    this->normals.push_back(static_cast<float>(d.Y()));
    this->normals.push_back(static_cast<float>(d.Z()));
  }

  // indices
  int n1, n2, n3;
  TopAbs_Orientation orient = face.Orientation();
  const Poly_Array1OfTriangle &triangles = triangulation->Triangles();
  for (i = 1; i <= triangulation->NbTriangles(); ++i) {
    triangles(i).Get(n1, n2, n3);
    if (orient != TopAbs_FORWARD) {
      int tmp = n1;
      n1 = n2;
      n2 = tmp;
    }
    if (this->isValid(nodes(n1), nodes(n2), nodes(n3))) {
      this->indices.push_back(--n1 + iDelta);
      this->indices.push_back(--n2 + iDelta);
      this->indices.push_back(--n3 + iDelta);
    }
  }
}

/**
 * Get Bb
 */
void Triangulation::getBb(double *min, double *max) const {
  *min = this->minBb;
  *max = this->maxBb;
}

/**
 * Get vertices
 * @returns Vertices
 */
std::vector<float> Triangulation::getVertices() const { return this->vertices; }

/**
 * Get normals
 * @returns normals
 */
std::vector<float> Triangulation::getNormals() const { return this->normals; }

/**
 * Get indices
 * @returns indices
 */
std::vector<uint> Triangulation::getIndices() const { return this->indices; }

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
