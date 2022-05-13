#include "StepReader.hpp"

#include <fstream>

#include <BRep_Builder.hxx>
#include <IFSelect_ReturnStatus.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <TDataStd_Name.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Compound.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>

#include "../logger/Logger.hpp"

/**
 * Constructor
 */
StepReader::StepReader() = default;

/**
 * Constructor
 * @param fileName File name
 */
StepReader::StepReader(const std::string &fileName) : m_fileName(fileName) {}

/**
 * Read
 * @returns Status
 */
bool StepReader::read() {
  IFSelect_ReturnStatus status;

  // Application
  Handle(XCAFApp_Application) app = XCAFApp_Application::GetApplication();

  if (app->NbDocuments() > 0) {
    app->GetDocument(1, this->m_document);
    app->Close(this->m_document);
  }

  app->NewDocument("STEP-XCAF", this->m_document);

  // Reader
  STEPCAFControl_Reader caf_reader = STEPCAFControl_Reader();
  status = caf_reader.ReadFile(this->m_fileName.c_str());
  if (status != IFSelect_RetDone) {
    Logger::ERROR("Unable to read " + this->m_fileName);
    return false;
  }

  // Document
  if (!caf_reader.Transfer(this->m_document)) {
    Logger::ERROR("Unable to transfert root");
    return false;
  }

  // Shape
  Handle(XCAFDoc_ShapeTool) shapeContent =
      XCAFDoc_DocumentTool::ShapeTool(this->m_document->Main());
  shapeContent->GetFreeShapes(this->m_labels);

  TopoDS_Compound compound;
  BRep_Builder builder;
  builder.MakeCompound(compound);
  for (TDF_LabelSequence::Iterator iter(this->m_labels); iter.More();
       iter.Next()) {
    const TDF_Label &label = iter.Value();
    TopoDS_Shape shape;
    if (XCAFDoc_ShapeTool::GetShape(label, shape)) {
      builder.Add(compound, shape);
    }
  }
  this->m_shape = compound;

  for (int i = 1; i <= this->m_labels.Size(); ++i) {
    if (XCAFDoc_ShapeTool::IsFree(this->m_labels.Value(i)))
      this->m_shapes.push_back(
          XCAFDoc_ShapeTool::GetShape(this->m_labels.Value(i)));
  }

  // Names
  std::cout << this->m_labels.Size() << std::endl;
  for (int i = 1; i <= this->m_labels.Size(); ++i) {
    TDF_Label label = this->m_labels.Value(i);
    Handle(TDataStd_Name) name;
    if (label.FindAttribute(TDataStd_Name::GetID(), name)) {
      TDataStd_Name::Set(label, "Solid_1"); // TODO correct string with i
    }
    // TODO remove non solid or faces or edges ?
    std::cout << label.NbChildren() << std::endl;
    // TDF_Label child = aLabel.FindChild(i);
  }

  return true;
}

std::vector<TopoDS_Shape> StepReader::getShapes() const {
  return this->m_shapes;
}

TopoDS_Shape StepReader::getShape() const { return this->m_shape; }

/**
 * Get document
 * @returns Document
 */
Handle(TDocStd_Document) StepReader::getDocument() const {
  return this->m_document;
}

/**
 * Get labels
 * @return Labels
 */
TDF_LabelSequence StepReader::getLabels() const { return this->m_labels; }