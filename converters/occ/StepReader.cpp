#include "StepReader.hpp"

#include <fstream>

#include <IFSelect_ReturnStatus.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>

/**
 * Constructor
 */
StepReader::StepReader() : fileName(""), shape() {}

/**
 * Constructor
 * @parem fileName File name
 */
StepReader::StepReader(std::string &fileName) : fileName(fileName), shape() {}

/**
 * Destructor
 */
StepReader::~StepReader() { this->fileName = ""; }

/**
 * Read
 * @returns Status
 */
bool StepReader::read() {
  Handle(XCAFApp_Application) app = XCAFApp_Application::GetApplication();
  // Handle(TDocStd_Document) doc;

  if (app->NbDocuments() > 0) {
    app->GetDocument(1, this->document);
    app->Close(this->document);
  }

  app->NewDocument("STEP-XCAF", this->document);

  STEPControl_Reader reader = STEPControl_Reader();
  IFSelect_ReturnStatus status = reader.ReadFile(fileName.c_str());
  if (status != IFSelect_RetDone) {
    std::cerr << "Unable to read " << fileName << std::endl;
    return false;
  }

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

  reader.NbRootsForTransfer();
  reader.TransferRoots();

  this->shape = reader.OneShape();

  // Handle(XCAFDoc_ShapeTool) shapeContent =
  //     XCAFDoc_DocumentTool::ShapeTool(this->document->Main());
  // TDF_LabelSequence shapes;
  // shapeContent->GetShapes(shapes);

  // if (shapes.Size() > 1)
  //   std::cout << "do not works if there are more than one shape" <<
  //   std::endl;

  // this->shape = shapeContent->GetShape(shapes.Value(1));

  return true;
}

/**
 * Get shape
 * @returns Shape
 */
TopoDS_Shape StepReader::getShape() const { return this->shape; }

/**
 * Get document
 * @returns Document
 */
Handle(TDocStd_Document) StepReader::getDocument() const {
  return this->document;
}
