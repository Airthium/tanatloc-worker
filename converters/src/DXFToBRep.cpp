#include <iostream>
#include <memory>

#include "dxf/DXFConverter.hpp"

#include "logger/Logger.hpp"

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

  return EXIT_SUCCESS;
}
