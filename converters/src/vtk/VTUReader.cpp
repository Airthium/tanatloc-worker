#include <tuple>

#include <vtkCellArray.h>
#include <vtkIdList.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkUnstructuredGrid.h>

#include "VTUReader.hpp"

#include "../logger/Logger.hpp"

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
 */
bool VTUReader::read() {
  reader->SetFileName(m_fileName.c_str());
  reader->Update();

  vtkSmartPointer<vtkUnstructuredGrid> output = reader->GetOutput();

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
    baseData.vertices.push_back((float)point[0]);
    baseData.vertices.push_back((float)point[1]);
    baseData.vertices.push_back((float)point[2]);
  }

  // Indices
  vtkSmartPointer<vtkCellArray> connectivity = output->GetCells();
  const auto numberOfCells = (int)connectivity->GetNumberOfCells();
  for (int i = 0; i < numberOfCells; ++i) {
    auto cellSize = connectivity->GetCellSize(i);
    if (cellSize != 3)
      continue;

    vtkSmartPointer<vtkIdList> indices = vtkSmartPointer<vtkIdList>::New();
    connectivity->GetCellAtId(i, indices);

    for (int j = 0; j < cellSize; ++j) {
      const auto index = (int)indices->GetId(j);
      baseData.indices.push_back(index);
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
        data.values.push_back((float)values[k]);
    }

    arrays.push_back(data);
  }

  return true;
}

std::vector<RData> VTUReader::getArrays() const { return this->arrays; }