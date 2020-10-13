#ifndef _STEPWRITER_HPP_
#define _STEPWRITER_HPP_

#include <TopoDS_Shape.hxx>

class StepWriter {
private:
  std::string m_fileName = "";
  TopoDS_Shape m_shape;

public:
  // Constructor
  StepWriter();
  // Constructor
  StepWriter(const std::string &, TopoDS_Shape);

  // Write
  bool write() const;
};

#endif
