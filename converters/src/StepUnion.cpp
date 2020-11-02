#include <algorithm>
#include <iostream>

#include "logger/Logger.hpp"
#include "occ/StepReader.hpp"
#include "occ/StepWriter.hpp"
#include "occ/Union.hpp"
#include "occ/getElements.hpp"

int main(int argc, char *argv[]) {
  bool res;
  std::string input;
  std::string output;

  if (argc < 3) {
    Logger::ERROR("USAGE:");
    Logger::ERROR("./StepUnion input output");
    return EXIT_FAILURE;
  }
  input = argv[1];
  output = argv[2];

  auto reader = StepReader(input);
  res = reader.read();
  if (!res) {
    Logger::ERROR("Unable to load step file " + input);
    return EXIT_FAILURE;
  }
  std::vector<TopoDS_Shape> inputShapes = reader.getShapes();

  // Get solids
  std::vector<TopoDS_Shape> solids = std::vector<TopoDS_Shape>();
  std::for_each(inputShapes.begin(), inputShapes.end(),
                [&solids](const TopoDS_Shape &shape) {
                  std::vector<TopoDS_Shape> solidTemp = getSolids(shape);
                  std::copy(solidTemp.begin(), solidTemp.end(),
                            back_inserter(solids));
                });

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
    Logger::ERROR("Unable to write step file " + output);
    return EXIT_FAILURE;
  }

  return 0;
}
