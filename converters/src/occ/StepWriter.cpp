#include "StepWriter.hpp"

#include <STEPControl_StepModelType.hxx>
#include <STEPControl_Writer.hxx>

StepWriter::StepWriter() : fileName(""), shape() {}

StepWriter::StepWriter(std::string &fileName, TopoDS_Shape shape)
    : fileName(fileName), shape(shape) {}

StepWriter::~StepWriter() { this->fileName = ""; }

bool StepWriter::write() {
  int res;
  STEPControl_Writer writer = STEPControl_Writer();
  STEPControl_StepModelType type = STEPControl_AsIs;
  res = writer.Transfer(this->shape, type);
  if (res != IFSelect_RetDone) {
    std::cerr << "Unable to transfer" << std::endl;
    return false;
  }
  writer.Write(this->fileName.c_str());
  if (res != IFSelect_RetDone) {
    std::cerr << "Unable to write" << std::endl;
    return false;
  }
  return true;
}
