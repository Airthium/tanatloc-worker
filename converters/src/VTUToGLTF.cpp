#include "logger/Logger.hpp"
#include "occ/GLTFWriter.hpp"
#include "occ/MainDocument.hpp"
#include "occ/Triangulation.hpp"
#include "occ/makeCylinder.hpp"
#include "vtk/VTUReader.hpp"
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
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

  // // Tetrahedra
  // TopoDS_Compound tethradedra;
  // TopoDS_Builder tethradedraBuilder;
  // tethradedraBuilder.MakeCompound(tethradedra);

  // TDF_Label tethradedraLabel = mainDocument.addShape(tethradedra,
  // "Tetrahedra");

  // for (uint i = 0; i < result.tetrahedra.size(); i++) {
  //   Tetrahedron tetrahedron = result.tetrahedra.at(i);

  //   // Indices
  //   uint index1 = tetrahedron.getI1();
  //   uint index2 = tetrahedron.getI2();
  //   uint index3 = tetrahedron.getI3();
  //   uint index4 = tetrahedron.getI4();

  //   // Data
  //   double data1 = result.values.at(index1);
  //   double data2 = result.values.at(index2);
  //   double data3 = result.values.at(index3);
  //   double data4 = result.values.at(index4);

  //   // Vertices
  //   Vertex v1 = result.vertices.at(index1);
  //   Vertex v2 = result.vertices.at(index2);
  //   Vertex v3 = result.vertices.at(index3);
  //   Vertex v4 = result.vertices.at(index4);

  //   // Points
  //   gp_Pnt point1(v1.X(), v1.Y(), v1.Z());
  //   gp_Pnt point2(v2.X(), v2.Y(), v2.Z());
  //   gp_Pnt point3(v3.X(), v3.Y(), v3.Z());
  //   gp_Pnt point4(v4.X(), v4.Y(), v4.Z());

  //   // Vertices
  //   BRepBuilderAPI_MakeVertex vertex1(point1);
  //   BRepBuilderAPI_MakeVertex vertex2(point2);
  //   BRepBuilderAPI_MakeVertex vertex3(point3);
  //   BRepBuilderAPI_MakeVertex vertex4(point4);

  //   // Edges
  //   BRepBuilderAPI_MakeEdge edge1(vertex1.Vertex(), vertex2.Vertex());
  //   BRepBuilderAPI_MakeEdge edge2(vertex1.Vertex(), vertex3.Vertex());
  //   BRepBuilderAPI_MakeEdge edge3(vertex1.Vertex(), vertex4.Vertex());
  //   BRepBuilderAPI_MakeEdge edge4(vertex2.Vertex(), vertex3.Vertex());
  //   BRepBuilderAPI_MakeEdge edge5(vertex2.Vertex(), vertex4.Vertex());
  //   BRepBuilderAPI_MakeEdge edge6(vertex3.Vertex(), vertex4.Vertex());

  //   // Cylinder
  //   TopoDS_Shape cylinder1 = makeCylinder(radius, edge1);
  //   TopoDS_Shape cylinder2 = makeCylinder(radius, edge2);
  //   TopoDS_Shape cylinder3 = makeCylinder(radius, edge3);
  //   TopoDS_Shape cylinder4 = makeCylinder(radius, edge4);
  //   TopoDS_Shape cylinder5 = makeCylinder(radius, edge5);
  //   TopoDS_Shape cylinder6 = makeCylinder(radius, edge6);

  //   tethradedraBuilder.Add(tethradedra, cylinder1);
  //   tethradedraBuilder.Add(tethradedra, cylinder2);
  //   tethradedraBuilder.Add(tethradedra, cylinder3);
  //   tethradedraBuilder.Add(tethradedra, cylinder4);
  //   tethradedraBuilder.Add(tethradedra, cylinder5);
  //   tethradedraBuilder.Add(tethradedra, cylinder6);

  //   std::string name1 = "Tetrahedron" + std::to_string(i) + ";1;{" +
  //                       std::to_string(data1) + "," + std::to_string(data2) +
  //                       "}";
  //   std::string name2 = "Tetrahedron" + std::to_string(i) + ";2;{" +
  //                       std::to_string(data1) + "," + std::to_string(data3) +
  //                       "}";
  //   std::string name3 = "Tetrahedron" + std::to_string(i) + ";3;{" +
  //                       std::to_string(data1) + "," + std::to_string(data4) +
  //                       "}";
  //   std::string name4 = "Tetrahedron" + std::to_string(i) + ";4;{" +
  //                       std::to_string(data2) + "," + std::to_string(data3) +
  //                       "}";
  //   std::string name5 = "Tetrahedron" + std::to_string(i) + ";5;{" +
  //                       std::to_string(data2) + "," + std::to_string(data4) +
  //                       "}";
  //   std::string name6 = "Tetrahedron" + std::to_string(i) + ";6;{" +
  //                       std::to_string(data3) + "," + std::to_string(data4) +
  //                       "}";

  //   mainDocument.addComponent(tethradedraLabel, cylinder1, name1);
  //   mainDocument.addComponent(tethradedraLabel, cylinder2, name2);
  //   mainDocument.addComponent(tethradedraLabel, cylinder3, name3);
  //   mainDocument.addComponent(tethradedraLabel, cylinder4, name4);
  //   mainDocument.addComponent(tethradedraLabel, cylinder5, name5);
  //   mainDocument.addComponent(tethradedraLabel, cylinder6, name6);
  // }

  // Triangles
  TopoDS_Compound triangles;
  TopoDS_Builder trianglesBuilder;
  trianglesBuilder.MakeCompound(triangles);

  TDF_Label trianglesLabel = mainDocument.addShape(triangles, "Triangles");

  for (uint i = 0; i < result.triangles.size(); i++) {
    Triangle triangle = result.triangles.at(i);

    // Indices
    uint index1 = triangle.getI1();
    uint index2 = triangle.getI2();
    uint index3 = triangle.getI3();

    // Data
    double data1 = result.values.at(index1);
    double data2 = result.values.at(index2);
    double data3 = result.values.at(index3);

    // Vertices
    Vertex v1 = result.vertices.at(index1);
    Vertex v2 = result.vertices.at(index2);
    Vertex v3 = result.vertices.at(index3);

    // Points
    gp_Pnt point1(v1.X(), v1.Y(), v1.Z());
    gp_Pnt point2(v2.X(), v2.Y(), v2.Z());
    gp_Pnt point3(v3.X(), v3.Y(), v3.Z());

    // Vertices
    BRepBuilderAPI_MakeVertex vertex1(point1);
    BRepBuilderAPI_MakeVertex vertex2(point2);
    BRepBuilderAPI_MakeVertex vertex3(point3);

    // Edges
    BRepBuilderAPI_MakeEdge edge1(vertex1.Vertex(), vertex2.Vertex());
    BRepBuilderAPI_MakeEdge edge2(vertex1.Vertex(), vertex3.Vertex());
    BRepBuilderAPI_MakeEdge edge3(vertex2.Vertex(), vertex3.Vertex());

    // Cylinder
    TopoDS_Shape cylinder1 = makeCylinder(radius, edge1);
    TopoDS_Shape cylinder2 = makeCylinder(radius, edge2);
    TopoDS_Shape cylinder3 = makeCylinder(radius, edge3);

    trianglesBuilder.Add(triangles, cylinder1);
    trianglesBuilder.Add(triangles, cylinder2);
    trianglesBuilder.Add(triangles, cylinder3);

    std::string name1 = "Triangle" + std::to_string(i) + ";1;{" +
                        std::to_string(data1) + "," + std::to_string(data2) +
                        "}";
    std::string name2 = "Triangle" + std::to_string(i) + ";2;{" +
                        std::to_string(data1) + "," + std::to_string(data3) +
                        "}";
    std::string name3 = "Triangle" + std::to_string(i) + ";3;{" +
                        std::to_string(data2) + "," + std::to_string(data3) +
                        "}";

    mainDocument.addComponent(trianglesLabel, cylinder1, name1);
    mainDocument.addComponent(trianglesLabel, cylinder2, name2);
    mainDocument.addComponent(trianglesLabel, cylinder3, name3);
  }

  // Triangulate
  Triangulation triangulation(mainDocument);
  triangulation.triangulate();

  // Write GLTF
  Handle(TDocStd_Document) document = mainDocument.document;
  TDF_LabelSequence labels = mainDocument.getLabels();
  auto writer = GLTFWriter(gltfFile, document, labels);
  bool res = writer.write();
  if (!res) {
    Logger::ERROR("Unable to write glft file " + gltfFile);
    return EXIT_FAILURE;
  }

  Logger::DISP("{ \"glb\": \"" + gltfFile + "\", \"name\": \"" + result.name +
               "\"}");

  return true;
}