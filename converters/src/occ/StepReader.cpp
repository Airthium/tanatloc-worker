#include "StepReader.hpp"

#include <fstream>

#include <IFSelect_ReturnStatus.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <STEPControl_Reader.hxx>
#include <StepData_Protocol.hxx>
#include <StepData_StepModel.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>

#include "../logger/Logger.hpp"

/**
 * Constructor
 */
StepReader::StepReader() {}

/**
 * Constructor
 * @parem fileName File name
 */
StepReader::StepReader(const std::string &fileName) : m_fileName(fileName) {}

/**
 * Read
 * @returns Status
 */
bool StepReader::read() {
  IFSelect_ReturnStatus status;

  // Document
  Handle(XCAFApp_Application) app = XCAFApp_Application::GetApplication();

  if (app->NbDocuments() > 0) {
    app->GetDocument(1, this->m_document);
    app->Close(this->m_document);
  }

  app->NewDocument("STEP-XCAF", this->m_document);

  // STEP CAF reader
  STEPCAFControl_Reader caf_reader = STEPCAFControl_Reader();
  status = caf_reader.ReadFile(this->m_fileName.c_str());
  if (status != IFSelect_RetDone) {
    Logger::ERROR("Unable to read " + this->m_fileName);
    return false;
  }

  if (!caf_reader.Transfer(this->m_document)) {
    Logger::ERROR("Unable to transfert root");
    return false;
  }

  Handle(XCAFDoc_ShapeTool) shapeContent =
      XCAFDoc_DocumentTool::ShapeTool(this->m_document->Main());
  TDF_LabelSequence shapes;
  shapeContent->GetShapes(shapes);

  for (int i = 1; i <= shapes.Size(); ++i) {
    if (XCAFDoc_ShapeTool::IsFree(shapes.Value(i)))
      this->m_shapes.push_back(XCAFDoc_ShapeTool::GetShape(shapes.Value(i)));
  }

  return true;
}

/**
 * Get shape
 * @returns Shape
 */
std::vector<TopoDS_Shape> StepReader::getShapes() const {
  return this->m_shapes;
}

/**
 * Get document
 * @returns Document
 */
Handle(TDocStd_Document) StepReader::getDocument() const {
  return this->m_document;
}
