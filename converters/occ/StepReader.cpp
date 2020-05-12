#include "StepReader.hpp"

#include <fstream>

#include <XCAFApp_Application.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <IFSelect_ReturnStatus.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_ColorTool.hxx>

/**
 * Constructor
 */
StepReader::StepReader () : fileName(""), shape() {}

/**
 * Constructor
 * @parem fileName File name
 */
StepReader::StepReader (std::string &fileName) : fileName(fileName), shape() {}

/**
 * Destructor
 */
StepReader::~StepReader () {
  this->fileName = "";
}

/**
 * Read
 * @returns Status
 */
bool StepReader::read () {
  Handle(XCAFApp_Application) app = XCAFApp_Application::GetApplication();
  // Handle(TDocStd_Document) doc;

  if (app->NbDocuments() > 0) {
    app->GetDocument(1, this->document);
    app->Close(this->document);
  }

  app->NewDocument("STEP-XCAF", this->document);

  STEPCAFControl_Reader reader = STEPCAFControl_Reader();
  IFSelect_ReturnStatus status = reader.ReadFile(fileName.c_str());

  if (status != IFSelect_RetDone) {
    std::cerr << "Unable to read " << fileName << std::endl;
    return false;
  }

  if (!reader.Transfer(this->document)) {
    std::cerr << "Unable to transfert root" << std::endl;
    return false;
  }

  Handle(XCAFDoc_ShapeTool) shapeContent = XCAFDoc_DocumentTool::ShapeTool(this->document->Main());
  TDF_LabelSequence shapes;
  shapeContent->GetShapes(shapes);

  this->shape = shapeContent->GetShape(shapes.Value(1));

  return true;
}

/**
 * Get shape
 * @returns Shape
 */
TopoDS_Shape StepReader::getShape () const {
  return this->shape;
}

/**
 * Get document
 * @returns Document
 */
Handle(TDocStd_Document) StepReader::getDocument () const {
  return this->document;
}
