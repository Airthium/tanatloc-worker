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
  VTUData baseData;
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
    VTUData data = baseData;
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
 * Get arrays
 * @return Arrays
 */
std::vector<VTUData> VTUReader::getArrays() const { return this->m_arrays; }

std::vector<Surface> VTUReader::getSurfaces() const {
  std::vector<VTUData> arrays = this->m_arrays;
  std::vector<Surface> surfaces;

  std::for_each(arrays.begin(), arrays.end(), [&surfaces](VTUData &data) {
    // Surface triangles & vertices
    std::vector<Triangle> tempSurfaceTriangles = data.triangles;
    std::vector<Triangle> surfaceTriangles;
    std::vector<Vertex> surfaceVertices;
    std::vector<double> surfaceValues;

    // Surface vertices
    std::vector<std::pair<uint, uint>> newIndices;
    std::for_each(
        tempSurfaceTriangles.begin(), tempSurfaceTriangles.end(),
        [data, &surfaceVertices, &newIndices, &surfaceTriangles,
         &surfaceValues](const Triangle triangle) {
          const uint index1 = triangle.I1();
          const uint index2 = triangle.I2();
          const uint index3 = triangle.I3();

          Triangle newTriangle;

          auto find1 =
              std::find_if(newIndices.begin(), newIndices.end(),
                           [index1](const std::pair<uint, uint> newIndex) {
                             return newIndex.first == index1;
                           });
          if (find1 == newIndices.end()) {
            const uint newIndex1 = surfaceVertices.size();
            surfaceVertices.push_back(data.vertices.at(index1));
            if (data.size == 1) {
              surfaceValues.push_back(data.values.at(index1));
            } else if (data.size == 3) {
              surfaceValues.push_back(data.values.at(3 * index1 + 0));
              surfaceValues.push_back(data.values.at(3 * index1 + 1));
              surfaceValues.push_back(data.values.at(3 * index1 + 2));
            }
            newTriangle.setI1(newIndex1);
            newIndices.push_back({index1, newIndex1});
          } else {
            newTriangle.setI1((*find1).second);
          }

          auto find2 =
              std::find_if(newIndices.begin(), newIndices.end(),
                           [index2](const std::pair<uint, uint> oldIndex) {
                             return oldIndex.first == index2;
                           });
          if (find2 == newIndices.end()) {
            const uint newIndex2 = surfaceVertices.size();
            surfaceVertices.push_back(data.vertices.at(index2));
            if (data.size == 1) {
              surfaceValues.push_back(data.values.at(index2));
            } else if (data.size == 3) {
              surfaceValues.push_back(data.values.at(3 * index2 + 0));
              surfaceValues.push_back(data.values.at(3 * index2 + 1));
              surfaceValues.push_back(data.values.at(3 * index2 + 2));
            }
            newTriangle.setI2(newIndex2);
            newIndices.push_back({index2, newIndex2});
          } else {
            newTriangle.setI2((*find2).second);
          }

          auto find3 =
              std::find_if(newIndices.begin(), newIndices.end(),
                           [index3](const std::pair<uint, uint> oldIndex) {
                             return oldIndex.first == index3;
                           });
          if (find3 == newIndices.end()) {
            const uint newIndex3 = surfaceVertices.size();
            surfaceVertices.push_back(data.vertices.at(index3));
            if (data.size == 1) {
              surfaceValues.push_back(data.values.at(index3));
            } else if (data.size == 3) {
              surfaceValues.push_back(data.values.at(3 * index3 + 0));
              surfaceValues.push_back(data.values.at(3 * index3 + 1));
              surfaceValues.push_back(data.values.at(3 * index3 + 2));
            }
            newTriangle.setI3(newIndex3);
            newIndices.push_back({index3, newIndex3});
          } else {
            newTriangle.setI3((*find3).second);
          }

          surfaceTriangles.push_back(newTriangle);
        });

    // min / max
    uint minIndex = 0; // Min is always 0
    uint maxIndex = 0;
    std::for_each(surfaceTriangles.begin(), surfaceTriangles.end(),
                  [&maxIndex](const Triangle triangle) {
                    const uint index1 = triangle.I1();
                    const uint index2 = triangle.I2();
                    const uint index3 = triangle.I3();

                    maxIndex = std::max(
                        maxIndex, std::max(index1, std::max(index2, index3)));
                  });

    Vertex minVertex(surfaceVertices.at(0));
    Vertex maxVertex(surfaceVertices.at(0));
    std::for_each(surfaceVertices.begin(), surfaceVertices.end(),
                  [&minVertex, &maxVertex](const Vertex vertex) {
                    const double x = vertex.X();
                    const double y = vertex.Y();
                    const double z = vertex.Z();

                    minVertex.setX(std::min(minVertex.X(), x));
                    minVertex.setY(std::min(minVertex.Y(), y));
                    minVertex.setZ(std::min(minVertex.Z(), z));

                    maxVertex.setX(std::max(maxVertex.X(), x));
                    maxVertex.setY(std::max(maxVertex.Y(), y));
                    maxVertex.setZ(std::max(maxVertex.Z(), z));
                  });

    double minValue = data.values.at(0);
    double maxValue = data.values.at(0);
    std::for_each(data.values.begin(), data.values.end(),
                  [&minValue, &maxValue](const double value) {
                    minValue = std::min(minValue, value);
                    maxValue = std::max(maxValue, value);
                  });

    Surface surface;
    surface.size = data.size;
    surface.name = data.name;
    surface.minIndex = minIndex;
    surface.maxIndex = maxIndex;
    surface.minVertex = minVertex;
    surface.maxVertex = maxVertex;
    surface.minValue = minValue;
    surface.maxValue = maxValue;
    surface.triangles = surfaceTriangles;
    surface.vertices = surfaceVertices;
    surface.values = surfaceValues;

    surfaces.push_back(surface);
  });

  return surfaces;
}
