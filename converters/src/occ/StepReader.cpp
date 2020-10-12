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

/**
 * Constructor
 */
StepReader::StepReader() : fileName(""), shapes() {}

/**
 * Constructor
 * @parem fileName File name
 */
StepReader::StepReader(std::string &fileName) : fileName(fileName), shapes() {}

/**
 * Destructor
 */
StepReader::~StepReader() {
  this->fileName = "";
  this->shapes.clear();
}

/**
 * Read
 * @returns Status
 */
bool StepReader::read() {
  IFSelect_ReturnStatus status;

  // Document
  Handle(XCAFApp_Application) app = XCAFApp_Application::GetApplication();

  if (app->NbDocuments() > 0) {
    app->GetDocument(1, this->document);
    app->Close(this->document);
  }

  app->NewDocument("STEP-XCAF", this->document);

  // STEP reader
  STEPControl_Reader reader = STEPControl_Reader();
  status = reader.ReadFile(fileName.c_str());
  if (status != IFSelect_RetDone) {
    std::cerr << "Unable to read " << fileName << std::endl;
    return false;
  }

  Handle(StepData_StepModel) model = reader.StepModel();
  Handle(StepData_Protocol) protocol =
      Handle(StepData_Protocol)::DownCast(model->Protocol());
  std::cout << protocol->SchemaName() << std::endl;

  // STEP CAF reader
  STEPCAFControl_Reader caf_reader = STEPCAFControl_Reader();
  status = caf_reader.ReadFile(fileName.c_str());
  if (status != IFSelect_RetDone) {
    std::cerr << "Unable to read " << fileName << std::endl;
    return false;
  }

  if (!caf_reader.Transfer(this->document)) {
    std::cerr << "Unable to transfert root" << std::endl;
    return false;
  }

  Handle(XCAFDoc_ShapeTool) shapeContent =
      XCAFDoc_DocumentTool::ShapeTool(this->document->Main());
  TDF_LabelSequence shapes;
  shapeContent->GetShapes(shapes);

  std::cout << shapes.Size() << std::endl;

  for (int i = 1; i <= shapes.Size(); ++i) {
    if (shapeContent->IsFree(shapes.Value(i)))
      this->shapes.push_back(shapeContent->GetShape(shapes.Value(i)));
  }

  return true;
}

/**
 * Get shape
 * @returns Shape
 */
std::vector<TopoDS_Shape> StepReader::getShapes() const { return this->shapes; }

/**
 * Get document
 * @returns Document
 */
Handle(TDocStd_Document) StepReader::getDocument() const {
  return this->document;
}