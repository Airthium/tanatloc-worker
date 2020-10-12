#include <iostream>

#include "DXFConverter.hpp"

#include "logger/Logger.hpp"

int main(int argc, char **argv) {
  bool res;
  std::string dxfFile, brepFile;

  // Input arguments
  if (argc < 3) {
    Logger::ERROR("USAGE:");
    Logger::ERROR("DXFToBRep file.dxf file.brep");
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
    Logger::ERROR("Unable to convert " + dxfFile);
    return EXIT_FAILURE;
  }

  // Write
  res = converter->write();
  if (!res) {
    Logger::ERROR("Unable to write " + brepFile);
    return EXIT_FAILURE;
  }

  // Clean
  delete converter;

  // Bye
  return EXIT_SUCCESS;
}
