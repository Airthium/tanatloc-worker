#include "VTUReader.hpp"
#include "../logger/Logger.hpp"
#include <vtkCellArray.h>
#include <vtkIdList.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkUnstructuredGrid.h>

/**
 * Constructor
 */
VTUReader::VTUReader() = default;

/**
 * Constructor
 */
VTUReader::VTUReader(const std::string &fileName) : m_fileName(fileName) {}

/**
 * Read
 * @return Status
 */
bool VTUReader::read() {
  // Check
  if (!this->m_reader->CanReadFile(m_fileName.c_str()))
    return false;

  // Read
  this->m_reader->SetFileName(m_fileName.c_str());
  this->m_reader->Update();

  vtkSmartPointer<vtkUnstructuredGrid> output = this->m_reader->GetOutput();

  const int numberOfPieces = output->GetNumberOfPieces();
  if (numberOfPieces != 1) {
    Logger::ERROR("Number of pieces != 1");
    return false;
  }

  // Geometry
  RData baseData;
  baseData.name = "base";
  baseData.size = 0;

  // Vertices
  vtkSmartPointer<vtkPoints> points = output->GetPoints();
  const auto numberOfPoints = (int)points->GetNumberOfPoints();
  for (int i = 0; i < numberOfPoints; ++i) {
    const double *point = points->GetPoint(i);
    Vertex v(point[0], point[1], point[2]);
    baseData.vertices.push_back(v);
  }

  // Indices
  vtkSmartPointer<vtkCellArray> connectivity = output->GetCells();
  const auto numberOfCells = (int)connectivity->GetNumberOfCells();
  for (int i = 0; i < numberOfCells; ++i) {
    auto cellSize = connectivity->GetCellSize(i);

    if (cellSize != 3 && cellSize != 4) // Only triangles & tetrahedra
      continue;

    vtkSmartPointer<vtkIdList> indices = vtkSmartPointer<vtkIdList>::New();
    connectivity->GetCellAtId(i, indices);

    if (cellSize == 3) { // Triangle
      Triangle triangle;

      const auto index1 = (int)indices->GetId(0);
      triangle.setI1(index1);
      const auto index2 = (int)indices->GetId(1);
      triangle.setI2(index2);
      const auto index3 = (int)indices->GetId(2);
      triangle.setI3(index3);

      baseData.triangles.push_back(triangle);
    } else if (cellSize == 4) { // Tetrahedron
      Tetrahedron tetrahedron;

      const auto index1 = (int)indices->GetId(0);
      tetrahedron.setI1(index1);
      const auto index2 = (int)indices->GetId(1);
      tetrahedron.setI2(index2);
      const auto index3 = (int)indices->GetId(2);
      tetrahedron.setI3(index3);
      const auto index4 = (int)indices->GetId(3);
      tetrahedron.setI4(index4);

      baseData.tetrahedra.push_back(tetrahedron);
    }
  }

  // Point data
  vtkSmartPointer<vtkPointData> pointData = output->GetPointData();
  const int numberOfPointData = pointData->GetNumberOfArrays();

  for (int i = 0; i < numberOfPointData; ++i) {
    RData data = baseData;
    data.name = pointData->GetArrayName(i);

    vtkSmartPointer<vtkDataArray> array = pointData->GetArray(i);
    const auto numberOfValues = (int)array->GetNumberOfValues();
    const int numberOfComponents = array->GetNumberOfComponents();
    data.size = numberOfComponents;

    for (int j = 0; j < numberOfValues; ++j) {
      const double *values = array->GetTuple(j);

      for (int k = 0; k < numberOfComponents; ++k)
        data.values.push_back(values[k]);
    }

    this->m_arrays.push_back(data);
  }

  return true;
}

/**
 * Get max dimension
 * @return Max
 */
double VTUReader::getMax() const {
  RData array = this->m_arrays.at(0);

  Vertex Min(array.vertices.at(0));
  Vertex Max(array.vertices.at(0));

  for (uint i = 1; i < array.vertices.size(); ++i) {
    double minX = Min.X();
    double minY = Min.Y();
    double minZ = Min.Z();

    double maxX = Max.X();
    double maxY = Max.Y();
    double maxZ = Max.Z();

    Vertex Current = array.vertices.at(i);
    double currentX = Current.X();
    double currentY = Current.Y();
    double currentZ = Current.Z();

    Min.setX(std::min(minX, currentX));
    Min.setY(std::min(minY, currentY));
    Min.setZ(std::min(minZ, currentZ));

    Max.setX(std::max(maxX, currentX));
    Max.setY(std::max(maxY, currentY));
    Max.setZ(std::max(maxZ, currentZ));
  }

  return std::max(Max.X() - Min.X(),
                  std::max(Max.Y() - Min.Y(), Max.Z() - Min.Z()));
}

/**
 * Get arrays
 * @return Arrays
 */
std::vector<RData> VTUReader::getArrays() const { return this->m_arrays; }