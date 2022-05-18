#ifndef _MAKE_CYLINDER_
#define _MAKE_CYLINDER_

#include <BRepBndLib.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRep_Tool.hxx>
#include <Bnd_Box.hxx>
#include <ShapeAnalysis_Edge.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Vertex.hxx>
#include <gp_Ax2.hxx>
#include <gp_Dir.hxx>
#include <gp_Pnt.hxx>

double computeMax(const TopoDS_Shape shape) {
  Bnd_Box boundingBox;
  double xMin;
  double yMin;
  double zMin;
  double xMax;
  double yMax;
  double zMax;

  BRepBndLib::Add(shape, boundingBox);
  boundingBox.Get(xMin, yMin, zMin, xMax, yMax, zMax);

  double xDim = std::abs(xMax - xMin);
  double yDim = std::abs(yMax - yMin);
  double zDim = std::abs(zMax - zMin);

  return std::max(xDim, std::max(yDim, zDim));
}

TopoDS_Shape makeCylinder(const double radius, const TopoDS_Shape edge) {
  // Analysis
  ShapeAnalysis_Edge analysis;

  // Vertices
  TopoDS_Vertex firstV = analysis.FirstVertex(TopoDS::Edge(edge));
  TopoDS_Vertex lastV = analysis.LastVertex(TopoDS::Edge(edge));

  // Points
  gp_Pnt first = BRep_Tool::Pnt(firstV);
  gp_Pnt last = BRep_Tool::Pnt(lastV);

  // Direction
  gp_Dir dir(last.X() - first.X(), last.Y() - first.Y(), last.Z() - first.Z());

  // Axe
  gp_Ax2 axe(first, dir);

  // Length
  float length = first.Distance(last);

  // Cylinder
  BRepPrimAPI_MakeCylinder makeCylinder(axe, radius, length);

  // Face
  return makeCylinder.Face();
}

TopoDS_Shape makeCylinder(const TopoDS_Shape parent, const TopoDS_Shape edge) {
  double maxBb = computeMax(parent);
  return makeCylinder(maxBb / 250., edge);
}

#endif //_MAKE_CYLINDER_