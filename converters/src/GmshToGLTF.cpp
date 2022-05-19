#include <algorithm>
#include <random>

#include "gmsh/Gmsh.hpp"
#include "logger/Logger.hpp"
#include "occ/GLTFWriter.hpp"
#include "occ/MainDocument.hpp"
#include "occ/Triangulation.hpp"
#include "occ/makeCylinder.hpp"
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <Quantity_Color.hxx>
#include <TopoDS_Builder.hxx>
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

  // Solids
  TopoDS_Compound solids;
  TopoDS_Builder solidsBuilder;
  solidsBuilder.MakeCompound(solids);

  TDF_Label solidsLabel = mainDocument.addShape(solids, "Solids");

  for (i = 0; i < mesh->getNumberOfTetrahedronLabels(); ++i) {
    // Vertices
    auto vertices = std::vector<Vertex>();
    std::for_each(volumesVertices[i].begin(), volumesVertices[i].end(),
                  [&vertices](const Vertex &v) { vertices.push_back(v); });

    // Color
    Quantity_Color color = generateColor();

    // Edges
    for (uint j = 0; j < vertices.size(); j += 4) {
      // Four points per tetrahedron
      gp_Pnt point1(vertices.at(j).X(), vertices.at(j).Y(), vertices.at(j).Z());
      gp_Pnt point2(vertices.at((j + 1)).X(), vertices.at((j + 1)).Y(),
                    vertices.at((j + 1)).Z());
      gp_Pnt point3(vertices.at((j + 2)).X(), vertices.at((j + 2)).Y(),
                    vertices.at((j + 2)).Z());
      gp_Pnt point4(vertices.at((j + 3)).X(), vertices.at((j + 3)).Y(),
                    vertices.at((j + 3)).Z());

      BRepBuilderAPI_MakeVertex vertex1(point1);
      BRepBuilderAPI_MakeVertex vertex2(point2);
      BRepBuilderAPI_MakeVertex vertex3(point3);
      BRepBuilderAPI_MakeVertex vertex4(point4);

      BRepBuilderAPI_MakeEdge edge1(vertex1.Vertex(), vertex2.Vertex());
      BRepBuilderAPI_MakeEdge edge2(vertex1.Vertex(), vertex3.Vertex());
      BRepBuilderAPI_MakeEdge edge3(vertex1.Vertex(), vertex4.Vertex());
      BRepBuilderAPI_MakeEdge edge4(vertex2.Vertex(), vertex3.Vertex());
      BRepBuilderAPI_MakeEdge edge5(vertex2.Vertex(), vertex4.Vertex());
      BRepBuilderAPI_MakeEdge edge6(vertex3.Vertex(), vertex4.Vertex());

      TopoDS_Shape cylinder1 = makeCylinder(radius, edge1);
      TopoDS_Shape cylinder2 = makeCylinder(radius, edge2);
      TopoDS_Shape cylinder3 = makeCylinder(radius, edge3);
      TopoDS_Shape cylinder4 = makeCylinder(radius, edge4);
      TopoDS_Shape cylinder5 = makeCylinder(radius, edge5);
      TopoDS_Shape cylinder6 = makeCylinder(radius, edge6);

      solidsBuilder.Add(solids, cylinder1);
      solidsBuilder.Add(solids, cylinder2);
      solidsBuilder.Add(solids, cylinder3);
      solidsBuilder.Add(solids, cylinder4);
      solidsBuilder.Add(solids, cylinder5);
      solidsBuilder.Add(solids, cylinder6);

      std::string name = "Solid " + std::to_string(i);

      mainDocument.addComponent(solidsLabel, cylinder1, color, name + " 1");
      mainDocument.addComponent(solidsLabel, cylinder2, color, name + " 2");
      mainDocument.addComponent(solidsLabel, cylinder3, color, name + " 3");
      mainDocument.addComponent(solidsLabel, cylinder4, color, name + " 4");
      mainDocument.addComponent(solidsLabel, cylinder5, color, name + " 5");
      mainDocument.addComponent(solidsLabel, cylinder6, color, name + " 6");
    }
  }

  // Faces
  TopoDS_Compound faces;
  TopoDS_Builder facesBuilder;
  facesBuilder.MakeCompound(faces);

  TDF_Label facesLabel = mainDocument.addShape(faces, "Faces");

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

      BRepBuilderAPI_MakeVertex vertex1(point1);
      BRepBuilderAPI_MakeVertex vertex2(point2);
      BRepBuilderAPI_MakeVertex vertex3(point3);

      BRepBuilderAPI_MakeEdge edge1(vertex1.Vertex(), vertex2.Vertex());
      BRepBuilderAPI_MakeEdge edge2(vertex1.Vertex(), vertex3.Vertex());
      BRepBuilderAPI_MakeEdge edge3(vertex2.Vertex(), vertex3.Vertex());

      TopoDS_Shape cylinder1 = makeCylinder(radius, edge1);
      TopoDS_Shape cylinder2 = makeCylinder(radius, edge2);
      TopoDS_Shape cylinder3 = makeCylinder(radius, edge3);

      facesBuilder.Add(faces, cylinder1);
      facesBuilder.Add(faces, cylinder2);
      facesBuilder.Add(faces, cylinder3);

      std::string name = "Face " + std::to_string(i);

      mainDocument.addComponent(facesLabel, cylinder1, color, name + " 1");
      mainDocument.addComponent(facesLabel, cylinder2, color, name + " 2");
      mainDocument.addComponent(facesLabel, cylinder3, color, name + " 3");
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
