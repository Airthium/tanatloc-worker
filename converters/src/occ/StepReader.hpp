#ifndef _STEPREADER_
#define _STEPREADER_

#include <vector>

#include <NCollection_Sequence.hxx>
#include <TDF_LabelSequence.hxx>
#include <TDocStd_Document.hxx>
#include <TopoDS_Shape.hxx>

class StepReader {
private:
  std::string m_fileName = "";
  std::vector<TopoDS_Shape> m_shapes;
  Handle(TDocStd_Document) m_document;
  TDF_LabelSequence m_labels;
  TopoDS_Shape m_shape;

public:
  // Constructor
  StepReader();
  // Constructor
  explicit StepReader(const std::string &);

  // Read
  bool read();

  // Get shapes
  std::vector<TopoDS_Shape> getShapes() const;

  // Get shape
  TopoDS_Shape getShape() const;

  // Get document
  Handle(TDocStd_Document) getDocument() const;

  // Get label sequence
  TDF_LabelSequence getLabels() const;
};

#endif
