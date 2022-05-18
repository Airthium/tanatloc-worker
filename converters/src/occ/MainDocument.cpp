#include "MainDocument.hpp"

#include <BRepBndLib.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRep_Tool.hxx>
#include <Bnd_Box.hxx>
#include <ShapeAnalysis_Edge.hxx>
#include <ShapeBuild_ReShape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Builder.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <gp_Ax2.hxx>
#include <gp_Dir.hxx>
#include <gp_Pnt.hxx>

// TopoDS_Shape MainDocument::makeCylinder(const TDF_Label &label,
//                                         const TopoDS_Shape edge) const {
//   // Parent shape
//   TopoDS_Shape parent = this->getShape(label);
//   Bnd_Box boundingBox;
//   double xMin;
//   double yMin;
//   double zMin;
//   double xMax;
//   double yMax;
//   double zMax;

//   BRepBndLib::Add(parent, boundingBox);
//   boundingBox.Get(xMin, yMin, zMin, xMax, yMax, zMax);

//   double xDim = std::abs(xMax - xMin);
//   double yDim = std::abs(yMax - yMin);
//   double zDim = std::abs(zMax - zMin);

//   double maxBb = std::max(xDim, std::max(yDim, zDim));

//   // Analysis
//   ShapeAnalysis_Edge analysis;

//   // Vertices
//   TopoDS_Vertex firstV = analysis.FirstVertex(TopoDS::Edge(edge));
//   TopoDS_Vertex lastV = analysis.LastVertex(TopoDS::Edge(edge));

//   // Points
//   gp_Pnt first = BRep_Tool::Pnt(firstV);
//   gp_Pnt last = BRep_Tool::Pnt(lastV);

//   // Direction
//   gp_Dir dir(last.X() - first.X(), last.Y() - first.Y(), last.Z() -
//   first.Z());

//   // Axe
//   gp_Ax2 axe(first, dir);

//   // Radius
//   // float radius = minBb / 50.;
//   float radius = maxBb / 250.;

//   // Length
//   float length = first.Distance(last);

//   // Cylinder
//   BRepPrimAPI_MakeCylinder makeCylinder(axe, radius, length);

//   // Face
//   return makeCylinder.Face();
// }

/**
 * Constructor
 */
MainDocument::MainDocument() {
  this->m_app = XCAFApp_Application::GetApplication();
  this->m_app->NewDocument("TANATLOC_CONVERTERS", this->document);

  this->m_shapeTool = XCAFDoc_DocumentTool::ShapeTool(this->document->Main());
  this->m_colorTool = XCAFDoc_DocumentTool::ColorTool(this->document->Main());
}

/**
 * Add shape
 * @param shape Shape
 * @return Label
 */
TDF_Label MainDocument::addShape(const TopoDS_Shape &shape) const {
  TDF_Label label = this->m_shapeTool->NewShape();
  this->m_shapeTool->SetShape(label, shape);
  return label;
}

/**
 * Add shape
 * @param shape Shape
 * @param color Color
 * @return Label
 */
TDF_Label MainDocument::addShape(const TopoDS_Shape &shape,
                                 const Quantity_Color &color) const {
  TDF_Label label = this->addShape(shape);
  this->m_colorTool->SetColor(label, color, XCAFDoc_ColorSurf);
  return label;
}

/**
 * Add subshape
 * @param label Label
 * @param shape Shape
 */
TDF_Label MainDocument::addComponent(const TDF_Label &label,
                                     const TopoDS_Shape &shape) const {
  return this->m_shapeTool->AddComponent(label, shape);
}

/**
 * Add subshape
 * @param label Label
 * @param shape Shape
 * @param color Color
 */
TDF_Label MainDocument::addComponent(const TDF_Label &label,
                                     const TopoDS_Shape &shape,
                                     const Quantity_Color &color) const {
  TDF_Label subLabel = this->m_shapeTool->AddComponent(label, shape);
  this->m_colorTool->SetColor(subLabel, color, XCAFDoc_ColorSurf);

  return subLabel;
}

/**
 * Get labels
 * @return Labels
 */
TDF_LabelSequence MainDocument::getLabels() const {
  TDF_LabelSequence labels;
  this->m_shapeTool->GetFreeShapes(labels);
  return labels;
}

/**
 * Get compound
 * @return Compound
 */
TopoDS_Compound MainDocument::getCompound() const {
  TDF_LabelSequence labels = this->getLabels();

  TopoDS_Compound compound;
  TopoDS_Builder builder;
  builder.MakeCompound(compound);
  for (uint i = 1; i <= labels.Size(); ++i) {
    const TDF_Label &label = labels.Value(i);
    TopoDS_Shape shape;
    if (XCAFDoc_ShapeTool::GetShape(label, shape)) {
      builder.Add(compound, shape);
    }
  }

  return compound;
}

TopoDS_Shape MainDocument::getShape(const TDF_Label &label) const {
  return this->m_shapeTool->GetShape(label);
}

void MainDocument::getComponents(const TDF_Label &label,
                                 TDF_LabelSequence &subLabels) const {
  this->m_shapeTool->GetComponents(label, subLabels);
}

/**
 * Get shape color
 * @param shape Shap
 * @return Color
 */
Quantity_Color MainDocument::getShapeColor(const TopoDS_Shape &shape) const {

  Quantity_Color color;
  this->m_colorTool->GetColor(shape, XCAFDoc_ColorSurf, color);

  if (color == OCCDefaultColor)
    color = TanatlocDefaultColor;

  return color;
}
