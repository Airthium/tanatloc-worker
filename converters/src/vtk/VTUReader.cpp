#include <tuple>

#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkIdList.h>
#include <vtkPointData.h>
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

  vtkUnstructuredGrid *output = reader->GetOutput();

  const int numberOfPieces = output->GetNumberOfPieces();
  if (numberOfPieces != 1) {
    Logger::ERROR("Number of pieces != 1");
    return false;
  }

  // Data
  // Point data
  auto pointData = std::vector<RData>();
  vtkSmartPointer<vtkPointData> pData = output->GetPointData();
  const int numberOfPointData = pData->GetNumberOfArrays();

  for (int i = 0; i < numberOfPointData; ++i) {
    RData data;
    data.name = pData->GetArrayName(i);

    vtkSmartPointer<vtkDataArray> array = pData->GetArray(i);
    const int numberOfValues = array->GetNumberOfValues();
    const int numberOfComponents = array->GetNumberOfComponents();

    data.size = numberOfComponents;

    for (int j = 0; j < numberOfValues; ++j) {
      double *tuple = array->GetTuple(j);

      for (int k = 0; k < numberOfComponents; ++k)
        data.values.push_back(tuple[k]);
    }

    pointData.push_back(data);
  }

  // Cell data
  auto cellData = std::vector<RData>();
  vtkSmartPointer<vtkCellData> cData = output->GetCellData();
  const int numberOfCellData = cData->GetNumberOfArrays();

  for (int i = 0; i < numberOfCellData; ++i) {
    RData data;
    data.name = cData->GetArrayName(i);

    vtkSmartPointer<vtkDataArray> array = cData->GetArray(i);
    const int numberOfValues = array->GetNumberOfValues();
    const int numberOfComponents = array->GetNumberOfComponents();

    data.size = numberOfComponents;

    for (int j = 0; j < numberOfValues; ++j) {
      double *tuple = array->GetTuple(j);

      for (int k = 0; k < numberOfComponents; ++k)
        data.values.push_back(tuple[k]);
    }

    cellData.push_back(data);
  }

  // Mesh
  const int numberOfCells = output->GetNumberOfCells();
  const int numberOfPoints = output->GetNumberOfPoints();

  auto points = std::vector<std::tuple<double, double, double>>();
  for (int i = 0; i < numberOfPoints; ++i) {
    double point[3];
    output->GetPoint(i, point);
    points.push_back({point[0], point[1], point[2]});
  }

  vtkCellArray *connectivity = output->GetCells();

  // Point data
  for (int i = 0; i < pointData.size(); ++i) {
    RData data;
    data.name = pointData[i].name;

    for (int j = 0; j < connectivity->GetNumberOfCells(); ++j) {
      int cellSize = connectivity->GetCellSize(j);

      // Only cellSize == 3 needed (surface)
      if (cellSize != 3)
        continue;

      vtkSmartPointer<vtkIdList> indices = vtkSmartPointer<vtkIdList>::New();
      connectivity->GetCellAtId(j, indices);

      for (int k = 0; k < cellSize; ++k) {
        const int index = indices->GetId(k);

        data.vertices.push_back(std::get<0>(points[index]));
        data.vertices.push_back(std::get<1>(points[index]));
        data.vertices.push_back(std::get<2>(points[index]));

        data.size = pointData[i].size;
        for (int l = 0; l < data.size; ++l)
          data.values.push_back(pointData[i].values[data.size * index + l]);
      }
    }

    this->arrays.push_back(data);
  }

  // Cell data
  for (int i = 0; i < cellData.size(); ++i) {
    RData data;
    data.name = cellData[i].name;

    for (int j = 0; j < connectivity->GetNumberOfCells(); ++j) {
      int cellSize = connectivity->GetCellSize(j);

      // Only cellSize == 3 needed (surface)
      if (cellSize != 3)
        continue;

      vtkSmartPointer<vtkIdList> indices = vtkSmartPointer<vtkIdList>::New();
      connectivity->GetCellAtId(j, indices);

      for (int k = 0; k < cellSize; ++k) {
        const int index = indices->GetId(k);

        data.vertices.push_back(std::get<0>(points[index]));
        data.vertices.push_back(std::get<1>(points[index]));
        data.vertices.push_back(std::get<2>(points[index]));

        data.size = pointData[i].size;
        for (int l = 0; l < data.size; ++l)
          data.values.push_back(cellData[i].values[data.size * index + l]);
      }
    }

    this->arrays.push_back(data);
  }

  return true;
}

std::vector<RData> VTUReader::getArrays() const { return this->arrays; }