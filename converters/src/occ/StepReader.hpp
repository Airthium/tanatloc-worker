#ifndef _STEPREADER_
#define _STEPREADER_

#include <vector>

#include <TDocStd_Document.hxx>
#include <TopoDS_Shape.hxx>

class StepReader {
private:
  std::string m_fileName = "";
  std::vector<TopoDS_Shape> m_shapes;
  Handle(TDocStd_Document) m_document;

public:
  // Constructor
  StepReader();
  // Constructor
  explicit StepReader(const std::string &);
  // Destructor
  ~StepReader();

  // Read
  bool read();

  // Get shape
  std::vector<TopoDS_Shape> getShapes() const;

  // Get document
  Handle(TDocStd_Document) getDocument() const;
};

#endif
