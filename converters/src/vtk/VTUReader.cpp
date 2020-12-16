#include <tuple>

#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkDataSetAttributes.h>
#include <vtkIdList.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkUnstructuredGrid.h>

#include "VTUReader.hpp"

#include "../logger/Logger.hpp"

/**
 * Constructor
 */
VTUReader::VTUReader() {}

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
  const int numberOfPoints = points->GetNumberOfPoints();
  for (int i = 0; i < numberOfPoints; ++i) {
    const double *point = points->GetPoint(i);
    baseData.vertices.push_back(point[0]);
    baseData.vertices.push_back(point[1]);
    baseData.vertices.push_back(point[2]);
  }

  // Indices
  vtkSmartPointer<vtkCellArray> connectivity = output->GetCells();
  const int numberOfCells = connectivity->GetNumberOfCells();
  for (int i = 0; i < numberOfCells; ++i) {
    int cellSize = connectivity->GetCellSize(i);
    if (cellSize != 3)
      continue;

    vtkSmartPointer<vtkIdList> indices = vtkSmartPointer<vtkIdList>::New();
    connectivity->GetCellAtId(i, indices);

    for (int j = 0; j < cellSize; ++j) {
      const int index = indices->GetId(j);
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
    const int numberOfValues = array->GetNumberOfValues();
    const int numberOfComponents = array->GetNumberOfComponents();
    data.size = numberOfComponents;

    for (int j = 0; j < numberOfValues; ++j) {
      double *values = array->GetTuple(j);

      for (int k = 0; k < numberOfComponents; ++k)
        data.values.push_back(values[k]);
    }

    arrays.push_back(data);
  }

  return true;
}

std::vector<RData> VTUReader::getArrays() const { return this->arrays; }