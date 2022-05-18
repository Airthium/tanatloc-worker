#include <iostream>
#include <memory>

#include "dxf/DXFConverter.hpp"
#include "logger/Logger.hpp"
#include "occ/GLTFWriter.hpp"
#include "occ/MainDocument.hpp"
#include "occ/Triangulation.hpp"
#include <BRepBuilderAPI_Copy.hxx>
#include <BRep_Builder.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>

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

  // Create OCC document
  TopoDS_Shape shape = converter->getShape();
  MainDocument mainDocument;

  TopExp_Explorer faceExplorer;
  for (faceExplorer.Init(shape, TopAbs_FACE); faceExplorer.More();
       faceExplorer.Next()) {
    TopoDS_Shape face = faceExplorer.Current();
    TDF_Label faceLabel = mainDocument.addShape(face);

    TopExp_Explorer edgeExplorer;
    for (edgeExplorer.Init(face, TopAbs_EDGE); edgeExplorer.More();
         edgeExplorer.Next()) {
      TopoDS_Shape edge = edgeExplorer.Current();
      mainDocument.addComponent(faceLabel, edge);
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
