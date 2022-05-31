#include <algorithm>

#include "logger/Logger.hpp"
// #include "occ/GLTFWriter.hpp"
#include "occ/MainDocument.hpp"
#include "occ/Triangulation.hpp"
#include "vtk/VTUReader.hpp"
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <TopoDS_Builder.hxx>
#include <TopoDS_Compound.hxx>

RData getMagnitude(const RData &);
RData getComponent(const RData &, const int);
bool writeOne(const RData &, const std::string &, const double);

int main(int argc, char *argv[]) {
  bool res;
  std::string vtuFile;
  std::string genericGltfFile;

  if (argc < 3) {
    Logger::ERROR("USAGE:");
    Logger::ERROR("./VTUToGLTF vtuFile genericGltfFile");
    return EXIT_FAILURE;
  }
  vtuFile = argv[1];
  genericGltfFile = argv[2];

  // Read VTU file
  auto reader = VTUReader(vtuFile);
  res = reader.read();
  if (!res) {
    Logger::ERROR("Unable to read VTU file " + vtuFile);
    return EXIT_FAILURE;
  }
  std::vector<RData> arrays = reader.getArrays();
  double radius = reader.getMax() / 250.;

  // Process arrays
  const auto numberOfArrays = (int)arrays.size();
  for (int i = 0; i < numberOfArrays; ++i) {
    RData result = arrays[i];

    if (result.size == 1) { // Scalar
      if (!writeOne(result, genericGltfFile + "_" + result.name + ".glb",
                    radius))
        return EXIT_FAILURE;
    } else if (result.size == 3) { // Vector
      // Magnitude
      RData magnitude = getMagnitude(result);
      if (!writeOne(magnitude,
                    genericGltfFile + "_" + result.name + "_magnitude.glb",
                    radius))
        return EXIT_FAILURE;

      // Component 1, 2 & 3
      for (int j = 0; j < 3; ++j) {
        RData component = getComponent(result, j);
        if (!writeOne(component,
                      genericGltfFile + "_" + result.name + "_component" +
                          std::to_string(j + 1) + ".glb",
                      radius))
          return EXIT_FAILURE;
      }
    }
  }

  return EXIT_SUCCESS;
}

/**
 * Get magnitude
 * @param result Result
 * @return Magnitude
 */
RData getMagnitude(const RData &result) {
  RData magnitude = result;
  magnitude.size = 1;
  magnitude.name = result.name + " (magnitude)";

  auto magnitudeValues = std::vector<double>();
  for (uint i = 0; i < magnitude.values.size() / 3; ++i) {
    auto v = sqrt(pow(magnitude.values[3 * i + 0], 2) +
                  pow(magnitude.values[3 * i + 1], 2) +
                  pow(magnitude.values[3 * i + 2], 2));
    magnitudeValues.push_back(v);
  }

  magnitude.values = magnitudeValues;

  return magnitude;
}

/**
 * Get component
 * @param result Result
 * @param index Index
 * @return Component
 */
RData getComponent(const RData &result, const int index) {
  RData component = result;
  component.size = 1;
  component.name =
      result.name + " (component " + std::to_string(index + 1) + ")";

  auto componentValues = std::vector<double>();
  for (uint i = 0; i < component.values.size() / 3; ++i) {
    double v = component.values[3 * i + index];
    componentValues.push_back(v);
  }

  component.values = componentValues;

  return component;
}

/**
 * Write one
 * @param result Result
 * @param gltfFile GLTF file
 * @param radius Radius
 * @return Status
 */
bool writeOne(const RData &result, const std::string &gltfFile,
              const double radius) {
  MainDocument mainDocument;
  std::string type = "result";
  // mainDocument.setType(type);

  // Triangles
  TopoDS_Compound triangles;
  TopoDS_Builder trianglesBuilder;
  trianglesBuilder.MakeCompound(triangles);

  // TDF_Label trianglesLabel = mainDocument.addShape(triangles, "Triangles");

  // for (uint i = 0; i < result.triangles.size(); i++) {
  //   Triangle triangle = result.triangles.at(i);

  //   // Indices
  //   uint index1 = triangle.I1();
  //   uint index2 = triangle.I2();
  //   uint index3 = triangle.I3();

  //   // Data
  //   double data1 = result.values.at(index1);
  //   double data2 = result.values.at(index2);
  //   double data3 = result.values.at(index3);

  //   // Vertices
  //   Vertex v1 = result.vertices.at(index1);
  //   Vertex v2 = result.vertices.at(index2);
  //   Vertex v3 = result.vertices.at(index3);

  //   // Points
  //   gp_Pnt point1(v1.X(), v1.Y(), v1.Z());
  //   gp_Pnt point2(v2.X(), v2.Y(), v2.Z());
  //   gp_Pnt point3(v3.X(), v3.Y(), v3.Z());

  //   // Polygon
  //   BRepBuilderAPI_MakePolygon polygonBuilder(point1, point2, point3, true);

  //   // Wire
  //   TopoDS_Wire wire = polygonBuilder.Wire();

  //   BRepBuilderAPI_MakeFace faceBuilder(wire);
  //   TopoDS_Shape face = faceBuilder.Shape();

  //   trianglesBuilder.Add(triangles, face);

  //   std::string data1str = std::to_string(data1);
  //   std::replace(data1str.begin(), data1str.end(), '.', ',');

  //   std::string data2str = std::to_string(data2);
  //   std::replace(data2str.begin(), data2str.end(), '.', ',');

  //   std::string data3str = std::to_string(data3);
  //   std::replace(data3str.begin(), data3str.end(), '.', ',');

  //   std::string name = data1str + ";" + data2str + ";" + data3str;

  //   mainDocument.addComponent(trianglesLabel, face, name);
  // }

  // // Triangulate
  // Triangulation triangulation(mainDocument);
  // triangulation.triangulate();

  // // Write GLTF
  // Handle(TDocStd_Document) document = mainDocument.document;
  // TDF_LabelSequence labels = mainDocument.getLabels();
  // auto writer = GLTFWriter(gltfFile, document, labels);
  // bool res = writer.write();
  // if (!res) {
  //   Logger::ERROR("Unable to write glft file " + gltfFile);
  //   return EXIT_FAILURE;
  // }

  // // Write description file
  // std::string descFile = gltfFile + ".desc";
  // res = mainDocument.writeDescription(descFile);
  // if (!res) {
  //   Logger::ERROR("Unable to write description file " + gltfFile + ".desc");
  //   return EXIT_FAILURE;
  // }

  // Logger::DISP("{ \"glb\": \"" + gltfFile + "\", \"name\": \"" + result.name
  // +
  //              "\"}");

  return true;
}