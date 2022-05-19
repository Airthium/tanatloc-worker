#include "MainDocument.hpp"

#include <BRepBndLib.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRep_Tool.hxx>
#include <Bnd_Box.hxx>
#include <ShapeAnalysis_Edge.hxx>
#include <ShapeBuild_ReShape.hxx>
#include <TDataStd_Name.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Builder.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <gp_Ax2.hxx>
#include <gp_Dir.hxx>
#include <gp_Pnt.hxx>

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
TDF_Label MainDocument::addShape(const TopoDS_Shape &shape,
                                 const std::string &name) const {
  TDF_Label label = this->m_shapeTool->NewShape();
  this->m_shapeTool->SetShape(label, shape);
  TDataStd_Name::Set(label, name.c_str());

  return label;
}

/**
 * Add shape
 * @param shape Shape
 * @param color Color
 * @return Label
 */
TDF_Label MainDocument::addShape(const TopoDS_Shape &shape,
                                 const Quantity_Color &color,
                                 const std::string &name) const {
  TDF_Label label = this->addShape(shape, name);
  this->m_colorTool->SetColor(label, color, XCAFDoc_ColorSurf);

  return label;
}

/**
 * Add subshape
 * @param label Label
 * @param shape Shape
 * @param name Name
 */
TDF_Label MainDocument::addComponent(const TDF_Label &label,
                                     const TopoDS_Shape &shape,
                                     const std::string &name) const {
  TDF_Label subLabel = this->m_shapeTool->AddComponent(label, shape, true);
  TDataStd_Name::Set(subLabel, name.c_str());

  return subLabel;
}

/**
 * Add subshape
 * @param label Label
 * @param shape Shape
 * @param color Color
 * @param name Name
 */
TDF_Label MainDocument::addComponent(const TDF_Label &label,
                                     const TopoDS_Shape &shape,
                                     const Quantity_Color &color,
                                     const std::string &name) const {
  TDF_Label subLabel = this->addComponent(label, shape, name);
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
