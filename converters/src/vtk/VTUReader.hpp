#ifndef _VTUREADER_HPP_
#define _VTUREADER_HPP_

#include <string>
#include <vector>

#include <vtkSmartPointer.h>
#include <vtkXMLUnstructuredGridReader.h>

struct RData {
  int size;
  std::string name;
  std::vector<uint> indices;
  std::vector<float> vertices;
  std::vector<float> values;
};

class VTUReader {
private:
  std::string m_fileName = "";
  vtkSmartPointer<vtkXMLUnstructuredGridReader> reader =
      vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();

  std::vector<RData> arrays = std::vector<RData>();

public:
  // Constructor
  VTUReader();
  // Constructor
  VTUReader(const std::string &);

  // Read
  bool read();

  // Get arrays
  std::vector<RData> getArrays() const;
};

#endif