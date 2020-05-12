#include <iostream>

#include "DXFConverter.hpp"

int main(int argc, char** argv) {
  bool res;
  std::string dxfFile, brepFile;

  // Input arguments
  if (argc < 3) {
    std::cerr << "USAGE:" << std::endl;
    std::cerr << "DXFToBRep file.dxf file.brep" << std::endl;
    return EXIT_FAILURE;
  }

  dxfFile = argv[1];
  brepFile = argv[2];

  // Converter
  DXFConverter *converter = new DXFConverter();

  // Set input
  converter->setInput(dxfFile);

  // Set output
  converter->setOutput(brepFile);

  // Convert
  res = converter->convert();
  if (!res) {
    std::cerr << "Unable to convert " << dxfFile << std::endl;
    return EXIT_FAILURE;
  }

  // Write
  res = converter->write();
  if (!res) {
    std::cerr << "Unable to write " << brepFile << std::endl;
    return EXIT_FAILURE;
  }

  // Clean
  delete converter;

  // Bye
  return EXIT_SUCCESS;
}
