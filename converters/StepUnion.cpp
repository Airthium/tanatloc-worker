#include <iostream>

#include "occ/StepReader.hpp"
#include "occ/StepWriter.hpp"
#include "occ/getElements.hpp"
#include "occ/Union.hpp"

int main (int argc, char *argv[]) {
  bool res;
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
  TopoDS_Shape inputShape = reader.getShape();

  // Get solids
  std::vector<TopoDS_Shape> solids = getSolids(inputShape);

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
