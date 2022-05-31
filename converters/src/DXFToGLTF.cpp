#include "dxf/DXFConverter.hpp"
#include "logger/Logger.hpp"
#include "occ/MainDocument.hpp"
#include "occ/Triangulation.hpp"
#include "occ/makePipe.hpp"
#include "utils/utils.hpp"
#include <TopExp_Explorer.hxx>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "tiny_gltf.h"

int main(int argc, char **argv) {
  bool res;
  std::string dxfFile;
  std::string gltfFile;

  // Input arguments
  if (argc < 3) {
    Logger::ERROR("USAGE:");
    Logger::ERROR("DXFToGLTF dxfFile glftFile");
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
  TopoDS_Compound compound = converter->getCompound();

  // Triangulate (prepare)
  Triangulation triangulation(compound);

  // GLTF
  tinygltf::Model model;
  tinygltf::Scene scene;
  tinygltf::Asset asset;

  uint nFaces = 0;
  uint nEdges = 0;

  // // Create document
  // MainDocument mainDocument;
  // mainDocument.setDimension(2);
  // std::string type = "geometry";
  // mainDocument.setType(type);

  // TopoDS_Compound faces;
  // TopoDS_Builder facesBuilder;
  // facesBuilder.MakeCompound(faces);

  // TopoDS_Compound edges;
  // TopoDS_Builder edgesBuilder;
  // edgesBuilder.MakeCompound(edges);
  // TDF_Label edgesLabel = mainDocument.addShape(edges, "Edges");

  // TopExp_Explorer faceExplorer;
  // for (faceExplorer.Init(shape, TopAbs_FACE); faceExplorer.More();
  //      faceExplorer.Next()) {
  //   // Face
  //   TopoDS_Shape face = faceExplorer.Current();
  //   facesBuilder.Add(faces, face);

  //   // New face
  //   mainDocument.add2DFace(face);

  //   TopExp_Explorer edgeExplorer;
  //   for (edgeExplorer.Init(face, TopAbs_EDGE); edgeExplorer.More();
  //        edgeExplorer.Next()) {
  //     // Edge
  //     TopoDS_Shape edge = edgeExplorer.Current();

  //     // Edge to pipe
  //     TopoDS_Shape pipe = makePipe(shape, TopoDS::Edge(edge));

  //     edgesBuilder.Add(edges, pipe);

  //     // New edge
  //     mainDocument.add2DEdge(edgesLabel, pipe);
  //   }
  // }

  // // Triangulate
  // Triangulation triangulation(mainDocument);
  // triangulation.triangulate();

  // // Write GLTF
  // Handle(TDocStd_Document) document = mainDocument.document;
  // TDF_LabelSequence labels = mainDocument.getLabels();
  // auto writer = GLTFWriter(gltfFile, document, labels);
  // res = writer.write();
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

  return EXIT_SUCCESS;
}
