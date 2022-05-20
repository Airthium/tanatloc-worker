#include <algorithm>
#include <random>

#include "gmsh/Gmsh.hpp"
#include "logger/Logger.hpp"
#include "occ/GLTFWriter.hpp"
#include "occ/MainDocument.hpp"
#include "occ/Triangulation.hpp"
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <Quantity_Color.hxx>
#include <TopoDS_Builder.hxx>
#include <TopoDS_Face.hxx>
#include <gp_Pnt.hxx>

Quantity_Color generateColor();

/**
 * Main function
 */
int main(int argc, char *argv[]) {
  bool res;
  uint i;
  std::string meshFile;
  std::string gltfFile;

  // Arguments
  if (argc < 3) {
    Logger::ERROR("USAGE:");
    Logger::ERROR("GmshToGLTF meshFile gltfFile");
    return EXIT_FAILURE;
  }
  meshFile = argv[1];
  gltfFile = argv[2];

  // Read & process mesh
  auto mesh = std::make_unique<Gmsh>();
  res = mesh->load(meshFile);
  if (!res) {
    Logger::ERROR("Unable to load Gmsh file " + meshFile);
    return EXIT_FAILURE;
  }

  mesh->computeLabels();

  std::vector<Vertex> *volumesVertices = mesh->getVolumesVertices();
  std::vector<Vertex> *surfacesVertices = mesh->getSurfacesVertices();

  double radius = mesh->getMax() / 250.;

  // Create document
  MainDocument mainDocument;

  // Triangles
  TopoDS_Compound triangles;
  TopoDS_Builder trianglesBuilder;
  trianglesBuilder.MakeCompound(triangles);

  TDF_Label facesLabel = mainDocument.addShape(triangles, "Triangles");

  for (i = 0; i < mesh->getNumberOfTriangleLabels(); ++i) {
    // Vertices
    auto vertices = std::vector<Vertex>();
    std::for_each(surfacesVertices[i].begin(), surfacesVertices[i].end(),
                  [&vertices](const Vertex &v) { vertices.push_back(v); });

    // Color
    Quantity_Color color = generateColor();

    // Edges
    for (uint j = 0; j < vertices.size(); j += 3) {
      // Three points per triangle
      gp_Pnt point1(vertices.at(j).X(), vertices.at(j).Y(), vertices.at(j).Z());
      gp_Pnt point2(vertices.at((j + 1)).X(), vertices.at((j + 1)).Y(),
                    vertices.at((j + 1)).Z());
      gp_Pnt point3(vertices.at((j + 2)).X(), vertices.at((j + 2)).Y(),
                    vertices.at((j + 2)).Z());

      BRepBuilderAPI_MakeVertex vertex1Builder(point1);
      BRepBuilderAPI_MakeVertex vertex2Builder(point2);
      BRepBuilderAPI_MakeVertex vertex3Builder(point3);

      TopoDS_Vertex vertex1 = vertex1Builder.Vertex();
      TopoDS_Vertex vertex2 = vertex2Builder.Vertex();
      TopoDS_Vertex vertex3 = vertex3Builder.Vertex();

      BRepBuilderAPI_MakeEdge edge1(vertex1, vertex2);
      BRepBuilderAPI_MakeEdge edge2(vertex2, vertex3);
      BRepBuilderAPI_MakeEdge edge3(vertex3, vertex1);

      BRepBuilderAPI_MakeWire wireBuilder(edge1.Edge(), edge2.Edge(),
                                          edge3.Edge());
      TopoDS_Wire wire = wireBuilder.Wire();

      BRepBuilderAPI_MakeFace faceBuilder(wire);
      TopoDS_Face face = faceBuilder.Face();

      trianglesBuilder.Add(triangles, face);

      std::string name =
          "label" + std::to_string(i) + ",number" + std::to_string(j);

      mainDocument.addComponent(facesLabel, face, color, name);
    }
  }

  // Triangulate
  Triangulation triangulation(mainDocument);
  triangulation.triangulate();

  // Write GLTF
  Handle(TDocStd_Document) document = mainDocument.document;
  TDF_LabelSequence labels = mainDocument.getLabels();
  auto writer = GLTFWriter(gltfFile, document, labels);
  res = writer.write();
  if (!res) {
    Logger::ERROR("Unable to write glft file " + gltfFile);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

/**
 * Generate random
 */
double generateRandom() {
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_real_distribution<> dist(0., 1.);
  return dist(generator);
}

/**
 * Generate random color
 */
Quantity_Color generateColor() {
  return Quantity_Color(generateRandom(), generateRandom(), generateRandom(),
                        Quantity_TOC_RGB);
}
