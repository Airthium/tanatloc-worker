#include <iostream>
#include <memory>

#include "dxf/DXFConverter.hpp"
#include "logger/Logger.hpp"
#include "occ/GLTFWriter.hpp"
#include "occ/MainDocument.hpp"
#include "occ/Triangulation.hpp"
#include <BRepBuilderAPI_Copy.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRep_Tool.hxx>
#include <ShapeAnalysis_Edge.hxx>
#include <ShapeBuild_ReShape.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Builder.hxx>
#include <TopoDS_Edge.hxx>
#include <gp_Ax2.hxx>
#include <gp_Dir.hxx>
#include <gp_Pnt.hxx>

TopoDS_Shape makeCylinder(const TopoDS_Shape edge) {

  // Analysis
  ShapeAnalysis_Edge analysis;

  // Vertices
  TopoDS_Vertex firstV = analysis.FirstVertex(TopoDS::Edge(edge));
  TopoDS_Vertex lastV = analysis.LastVertex(TopoDS::Edge(edge));

  // Points
  gp_Pnt first = BRep_Tool::Pnt(firstV);
  gp_Pnt last = BRep_Tool::Pnt(lastV);

  // Direction
  gp_Dir dir(last.X() - first.X(), last.Y() - first.Y(), last.Z() - first.Z());

  // Axe
  gp_Ax2 axe(first, dir);

  // Radius
  // float radius = minBb / 50.;
  float radius = 0.1;

  // Length
  float length = first.Distance(last);

  // Cylinder
  BRepPrimAPI_MakeCylinder makeCylinder(axe, radius, length);

  // Face
  return makeCylinder.Face();
}

int main(int argc, char **argv) {
  bool res;
  std::string dxfFile;
  std::string gltfFile;

  // Input arguments
  if (argc < 3) {
    Logger::ERROR("USAGE:");
    Logger::ERROR("DXFToBRep dxfFile glftFile");
    return EXIT_FAILURE;
  }
  dxfFile = argv[1];
  gltfFile = argv[2];

  // Converter
  auto converter = std::make_unique<DXFConverter>();
  converter->setInput(dxfFile);
  res = converter->convert();
  if (!res) {
    Logger::ERROR("Unable to convert " + dxfFile);
    return EXIT_FAILURE;
  }
  TopoDS_Shape shape = converter->getShape();

  // Create OCC document
  MainDocument mainDocument;

  TopoDS_Compound faces;
  TopoDS_Builder facesBuilder;
  facesBuilder.MakeCompound(faces);
  TDF_Label facesLabel = mainDocument.addShape(faces);

  TopoDS_Compound edges;
  TopoDS_Builder edgesBuilder;
  edgesBuilder.MakeCompound(edges);

  TopExp_Explorer faceExplorer;
  for (faceExplorer.Init(shape, TopAbs_FACE); faceExplorer.More();
       faceExplorer.Next()) {
    // Face
    TopoDS_Shape face = faceExplorer.Current();
    facesBuilder.Add(faces, face);

    // New face
    TDF_Label faceLabel = mainDocument.addComponent(facesLabel, face);

    TopExp_Explorer edgeExplorer;
    for (edgeExplorer.Init(face, TopAbs_EDGE); edgeExplorer.More();
         edgeExplorer.Next()) {
      // Edge
      TopoDS_Shape edge = edgeExplorer.Current();

      // Edge to pipe
      TopoDS_Shape pipe = makeCylinder(edge);
      edgesBuilder.Add(edges, pipe);
    }
  }

  // New edges
  mainDocument.addComponent(facesLabel, edges);

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
