#include <tuple>

#include <vtkCellArray.h>
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
bool VTUReader::read() const {
  reader->SetFileName(m_fileName.c_str());
  reader->Update();

  vtkUnstructuredGrid *output = reader->GetOutput();

  const int numberOfPieces = output->GetNumberOfPieces();
  if (numberOfPieces != 1) {
    Logger::ERROR("Number of pieces != 1");
    return false;
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
  for (int i = 0; i < connectivity->GetNumberOfCells(); ++i) {
    std::cout << connectivity->GetCellSize(i) << std::endl;
  }
  // TODO

  // Data

  //   // Cell arrays
  //   const int numberOfCellArrays = output->GetNumberOfCellArrays();
  //   for (int i = 0; i < numberOfCellArrays; ++i) {
  //     data newData;
  //     newData.name = reader->GetCellArrayName(i);

  //     arrays.push_back(newData);
  //   }

  //   std::cout << "nPointArrays: " << reader->GetNumberOfPointArrays()
  //             << std::endl;
  //   std::cout << reader->GetPointArrayName(0) << std::endl;
  //   std::cout << "nCellArrays: " << reader->GetNumberOfCellArrays() <<
  //   std::endl; std::cout << reader->GetCellArrayName(0) << std::endl;

  return true;
}