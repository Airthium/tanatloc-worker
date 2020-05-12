#ifndef _STEPWRITER_HPP_
#define _STEPWRITER_HPP_

#include <TopoDS_Shape.hxx>

class StepWriter {
private:
  std::string fileName;
  TopoDS_Shape shape;

public:
  // Constructor
  StepWriter();
  // Constructor
  StepWriter(std::string&, TopoDS_Shape);
  // Destructor
  ~StepWriter();

  // Write
  bool write();
};

#endif
