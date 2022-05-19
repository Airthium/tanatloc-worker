#ifndef _VTUREADER_HPP_
#define _VTUREADER_HPP_

#include <string>
#include <vector>

#include "../gmsh/Tetrahedron.hpp"
#include "../gmsh/Triangle.hpp"
#include "../gmsh/Vertex.hpp"
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

#endif