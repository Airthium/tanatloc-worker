#include <iostream>

#include "occ/StepReader.hpp"
#include "occ/StepWriter.hpp"
#include "occ/Union.hpp"
#include "occ/getElements.hpp"

int main(int argc, char *argv[]) {
  bool res;
  uint i;
  std::string input;
  std::string output;

  if (argc < 3) {
    std::cerr << "USAGE:" << std::endl;
    std::cerr << "./StepUnion input output" << std::endl;
    return EXIT_FAILURE;
  }
  input = argv[1];
  output = argv[2];

  StepReader reader = StepReader(input);
  res = reader.read();
  if (!res) {
    std::cerr << "Unable to load step file " << input << std::endl;
    return EXIT_FAILURE;
  }
  std::vector<TopoDS_Shape> inputShapes = reader.getShapes();

  // Get solids
  std::vector<TopoDS_Shape> solids = std::vector<TopoDS_Shape>();
  for (i = 0; i < inputShapes.size(); ++i) {
    std::vector<TopoDS_Shape> solidTemp = getSolids(inputShapes[i]);
    std::copy(solidTemp.begin(), solidTemp.end(), back_inserter(solids));
  }

  // Union
  Union fuse = Union();
  std::vector<TopoDS_Shape> objects = std::vector<TopoDS_Shape>();
  objects.push_back(solids[0]);
  fuse.setObjects(objects);
  solids.erase(solids.begin());
  fuse.setTools(solids);

  TopoDS_Shape outputShape = fuse.compute();

  // Write
  StepWriter writer = StepWriter(output, outputShape);
  res = writer.write();
  if (!res) {
    std::cerr << "Unable to write step file " << output << std::endl;
    return EXIT_FAILURE;
  }

  return 0;
}