#include "MainDocument.hpp"

#include <fstream>
#include <uuid/uuid.h>

#include "../logger/Logger.hpp"
#include <TDataStd_Name.hxx>
#include <TopoDS_Builder.hxx>
#include <XCAFDoc_DocumentTool.hxx>

std::string uuid() {
  uuid_t uuid;
  char uuidChar[UUID_STR_LEN];
  uuid_generate_random(uuid);
  uuid_unparse_upper(uuid, uuidChar);

  return std::string(uuidChar);
}

void DumpElement(std::ofstream &stream, Element &element) {
  stream << "\t{" << std::endl;
  stream << "\t\t\"name\": \"" << element.name << "\"," << std::endl;
  stream << "\t\t\"uuid\": \"" << element.uuid << "\"," << std::endl;
  stream << "\t\t\"label\": " << element.label << "," << std::endl;
  stream << "\t\t\"color\": {\"r\": " << element.color.r
         << ", \"g\": " << element.color.g << ", \"b\": " << element.color.b
         << "}" << std::endl;
  stream << "\t}";
}

/**
 * Constructor
 */
MainDocument::MainDocument() {
  this->m_app = XCAFApp_Application::GetApplication();
  this->m_app->NewDocument("TANATLOC_CONVERTERS", this->document);

  this->m_shapeTool = XCAFDoc_DocumentTool::ShapeTool(this->document->Main());
  this->m_colorTool = XCAFDoc_DocumentTool::ColorTool(this->document->Main());
}

void MainDocument::setDimension(int dimension) {
  this->m_dimension = dimension;
}

void MainDocument::setType(std::string &type) { this->m_type = type; }

TDF_Label MainDocument::add3DSolid(const TopoDS_Shape &solid) {
  int number = this->m_solids.size() + 1;
  std::string name = "Solid " + std::to_string(number);

  TDF_Label label = addShape(solid, name);

  Element element;
  element.name = name;
  element.label = number;
  element.uuid = uuid();

  this->m_solids.push_back(element);

  return label;
}

TDF_Label MainDocument::add3DSolid(const TopoDS_Shape &solid,
                                   const Quantity_Color &color) {
  int number = this->m_solids.size() + 1;
  std::string name = "Solid " + std::to_string(number);

  TDF_Label label = addShape(solid, color, name);

  Element element;
  element.name = name;
  element.label = number;
  element.uuid = uuid();

  Color elementColor;
  elementColor.r = color.Red();
  elementColor.g = color.Green();
  elementColor.b = color.Blue();

  element.color = elementColor;

  this->m_solids.push_back(element);

  return label;
}

TDF_Label MainDocument::add3DFace(const TDF_Label &solidLabel,
                                  const TopoDS_Shape &face) {
  int number = this->m_faces.size() + 1;
  std::string name = "Face " + std::to_string(number);

  TDF_Label label = addComponent(solidLabel, face, name);

  Element element;
  element.name = name;
  element.label = number;
  element.uuid = uuid();

  this->m_faces.push_back(element);

  return label;
}

TDF_Label MainDocument::add3DFace(const TDF_Label &solidLabel,
                                  const TopoDS_Shape &face,
                                  const Quantity_Color &color) {
  int number = this->m_faces.size() + 1;
  std::string name = "Face " + std::to_string(number);

  TDF_Label label = addComponent(solidLabel, face, color, name);

  Element element;
  element.name = name;
  element.label = number;
  element.uuid = uuid();

  Color elementColor;
  elementColor.r = color.Red();
  elementColor.g = color.Green();
  elementColor.b = color.Blue();

  element.color = elementColor;

  this->m_faces.push_back(element);

  return label;
}

TDF_Label MainDocument::add2DFace(const TopoDS_Shape &face) {
  int number = this->m_faces.size() + 1;
  std::string name = "Face " + std::to_string(number);

  TDF_Label label = addShape(face, name);

  Element element;
  element.name = name;
  element.label = number;
  element.uuid = uuid();

  this->m_faces.push_back(element);

  return label;
}

TDF_Label MainDocument::add2DEdge(const TDF_Label &edgesLabel,
                                  const TopoDS_Shape &edge) {
  int number = this->m_edges.size() + 1;
  std::string name = "Edge " + std::to_string(number);

  TDF_Label label = addComponent(edgesLabel, edge, name);

  Element element;
  element.name = name;
  element.label = number;
  element.uuid = uuid();

  this->m_edges.push_back(element);

  return label;
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

bool MainDocument::writeDescription(std::string &fileName) const {
  std::ofstream file;

  file.open(fileName, std::ios::out | std::ios::trunc);
  if (!file) {
    Logger::ERROR("Unable to open " + fileName);
    return false;
  }

  file << "{" << std::endl;
  file << "\t\"uuid\": \"" << uuid() << "\"," << std::endl;
  file << "\t\"type\": \"" << this->m_type << "\"," << std::endl;
  file << "\t\"dimension\": " << this->m_dimension << "," << std::endl;
  file << "\t\"solids\": [" << std::endl;

  size_t numberOfSolids = this->m_solids.size();
  for (size_t i = 0; i < numberOfSolids; ++i) {
    Element solid = this->m_solids.at(i);
    DumpElement(file, solid);
    if (i < numberOfSolids - 1)
      file << "," << std::endl;
    else
      file << std::endl;
  }

  file << "\t]," << std::endl;
  file << "\t\"faces\": [" << std::endl;

  size_t numberOfFaces = this->m_faces.size();
  for (size_t i = 0; i < numberOfFaces; ++i) {
    Element face = this->m_faces.at(i);
    DumpElement(file, face);
    if (i < numberOfFaces - 1)
      file << "," << std::endl;
    else
      file << std::endl;
  }

  file << "\t]," << std::endl;
  file << "\t\"edges\": [" << std::endl;

  size_t numberOfEdges = this->m_edges.size();
  for (size_t i = 0; i < numberOfEdges; ++i) {
    Element edge = this->m_edges.at(i);
    DumpElement(file, edge);
    if (i < numberOfEdges - 1)
      file << "," << std::endl;
    else
      file << std::endl;
  }

  file << "\t]" << std::endl;
  file << "}" << std::endl;

  file.close();

  return true;
}