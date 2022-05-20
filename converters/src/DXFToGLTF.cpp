#include "dxf/DXFConverter.hpp"
#include "logger/Logger.hpp"
#include "occ/GLTFWriter.hpp"
#include "occ/MainDocument.hpp"
#include "occ/Triangulation.hpp"
#include "occ/makePipe.hpp"
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Builder.hxx>

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
  TopoDS_Shape shape = converter->getShape();

  // Create document
  MainDocument mainDocument;

  TopoDS_Compound faces;
  TopoDS_Builder facesBuilder;
  facesBuilder.MakeCompound(faces);
  TDF_Label facesLabel = mainDocument.addShape(faces, "Faces");

  TopoDS_Compound edges;
  TopoDS_Builder edgesBuilder;
  edgesBuilder.MakeCompound(edges);

  uint nbFaces = 0;
  uint nbEdges = 0;
  TopExp_Explorer faceExplorer;
  for (faceExplorer.Init(shape, TopAbs_FACE); faceExplorer.More();
       faceExplorer.Next()) {
    // Face
    TopoDS_Shape face = faceExplorer.Current();
    facesBuilder.Add(faces, face);

    // New face
    nbFaces++;
    mainDocument.addComponent(facesLabel, face,
                              "Face" + std::to_string(nbFaces));

    TopExp_Explorer edgeExplorer;
    for (edgeExplorer.Init(face, TopAbs_EDGE); edgeExplorer.More();
         edgeExplorer.Next()) {
      // Edge
      TopoDS_Shape edge = edgeExplorer.Current();

      // Edge to pipe
      TopoDS_Shape pipe = makePipe(shape, TopoDS::Edge(edge));

      edgesBuilder.Add(edges, pipe);
    }
  }

  mainDocument.addComponent(facesLabel, edges, "Edges");

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
