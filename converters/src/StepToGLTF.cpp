#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include <sys/stat.h>
#include <sys/types.h>

#include "logger/Logger.hpp"
#include "occ/GLTFWriter.hpp"
#include "occ/StepReader.hpp"
#include "occ/Triangulation.hpp"

/**
 * Main function
 */
int main(int argc, char *argv[]) {
  bool res;
  uint i;
  std::string stepFile;
  std::string gltfFile;

  // Arguments
  if (argc < 3) {
    Logger::ERROR("USAGE:");
    Logger::ERROR("./StepToGLTF stepFile gltfFile");
    return EXIT_FAILURE;
  }
  stepFile = argv[1];
  gltfFile = argv[2];

  // Read step file
  auto reader = StepReader(stepFile);
  res = reader.read();
  if (!res) {
    Logger::ERROR("Unable to read step file " + stepFile);
    return EXIT_FAILURE;
  }
  MainDocument mainDocument = reader.getMainDocument();

  // Triangulate
  Triangulation triangulation(mainDocument);
  triangulation.triangulate();

  // Write gltf file
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
