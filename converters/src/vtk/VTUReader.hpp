#ifndef _VTUREADER_HPP_
#define _VTUREADER_HPP_

#include <string>
#include <vector>

#include <vtkSmartPointer.h>
#include <vtkXMLUnstructuredGridReader.h>

struct data {
  std::string name;
  std::vector<float> value;
};

class VTUReader {
private:
  std::string m_fileName = "";
  vtkSmartPointer<vtkXMLUnstructuredGridReader> reader =
      vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();

  std::vector<data> arrays = std::vector<data>();

public:
  // Constructor
  VTUReader();
  // Constructor
  VTUReader(const std::string &);

  // Read
  bool read() const;
};

#endif