#ifndef _VTU_READER_
#define _VTU_READER_

#include <string>
#include <vector>

#include "../geometry/Tetrahedron.hpp"
#include "../geometry/Triangle.hpp"
#include "../geometry/Vertex.hpp"
#include <vtkSmartPointer.h>
#include <vtkXMLUnstructuredGridReader.h>

struct RData {
  int size;
  std::string name;
  std::vector<Vertex> vertices;
  std::vector<Triangle> triangles;
  std::vector<Tetrahedron> tetrahedra;
  std::vector<double> values;
};

class VTUReader {
private:
  std::string m_fileName = "";
  vtkSmartPointer<vtkXMLUnstructuredGridReader> m_reader =
      vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();

  std::vector<RData> m_arrays = std::vector<RData>();

public:
  // Constructor
  VTUReader();
  // Constructor
  explicit VTUReader(const std::string &);

  // Read
  bool read();

  // Get max
  double getMax() const;

  // Get arrays
  std::vector<RData> getArrays() const;
};

#endif // VTU_READER_