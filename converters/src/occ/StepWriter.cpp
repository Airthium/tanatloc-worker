#include "StepWriter.hpp"

#include <STEPControl_StepModelType.hxx>
#include <STEPControl_Writer.hxx>

#include "../logger/Logger.hpp"

/**
 * Constructor
 */
StepWriter::StepWriter() {}

/**
 * Constructor
 */
StepWriter::StepWriter(const std::string &fileName, TopoDS_Shape shape)
    : m_fileName(fileName), m_shape(shape) {}

/**
 * Desctructor
 */
StepWriter::~StepWriter() { this->m_fileName = ""; }

/**
 * Write
 */
bool StepWriter::write() const {
  int res;
  STEPControl_Writer writer = STEPControl_Writer();
  STEPControl_StepModelType type = STEPControl_AsIs;
  res = writer.Transfer(this->m_shape, type);
  if (res != IFSelect_RetDone) {
    Logger::ERROR("Unable to transfer");
    return false;
  }
  writer.Write(this->m_fileName.c_str());
  if (res != IFSelect_RetDone) {
    Logger::ERROR("Unable to write");
    return false;
  }
  return true;
}
