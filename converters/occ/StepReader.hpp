#ifndef _STEPREADER_
#define _STEPREADER_

#include <vector>

#include <TopoDS_Shape.hxx>
#include <TDocStd_Document.hxx>

class StepReader {
private:
  std::string fileName;
  TopoDS_Shape shape;
  Handle(TDocStd_Document) document;

public:
  // Constructor
  StepReader();
  // Constructor
  explicit StepReader(std::string&);
  // Destructor
  ~StepReader();

  // Read
  bool read();

  // Get shape
  TopoDS_Shape getShape() const;

  // Get document
  Handle(TDocStd_Document) getDocument() const;
};

#endif
