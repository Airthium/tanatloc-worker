#include <sstream>

#include <sys/stat.h>
#include <sys/types.h>

#include "logger/Logger.hpp"
#include "threeJS/ThreeJS.hpp"
#include "vtk/VTUReader.hpp"

int main(int argc, char *argv[]) {
  bool res;
  std::string vtuFile;
  std::string threeJSPath;

  if (argc < 2) {
    Logger::ERROR("USAGE:");
    Logger::ERROR("./VTUToThreeJS vtuFile [threeJSPath]");
    return EXIT_FAILURE;
  }
  vtuFile = argv[1];

  if (argc >= 3) {
    threeJSPath = argv[2];
  } else {
    threeJSPath = vtuFile;
    size_t pos = threeJSPath.find_last_of(".");
    if (pos != std::string::npos) {
      threeJSPath = threeJSPath.substr(0, pos);
    }
  }

  // Create path
  int err = mkdir(threeJSPath.c_str(), S_IRWXU | S_IRWXG);
  if (err && errno != EEXIST) {
    Logger::ERROR("Unable to create threeJS path");
    return EXIT_FAILURE;
  }

  // Read VTU file
  auto reader = VTUReader(vtuFile);
  res = reader.read();
  if (!res) {
    Logger::ERROR("Unable to read VTU file " + vtuFile);
    return EXIT_FAILURE;
  }
  std::vector<RData> arrays = reader.getArrays();

  // Write ThreeJS files
  const int numberOfArrays =
      arrays.size() for (int i = 0; i < numberOfArrays; ++i) {
    ThreeJS part(arrays[i].vertices);
    part.setIndices(arrays[i].indices);
    part.setName(arrays[i].name);
    part.setData(arrays[i].values);

    std::ostringstream oss;
    oss << threeJSPath << "/" << FACE << i + 1 << ".json";
    if (!part.save(oss.str())) {
      Logger::ERROR("Unable to write ThreeJS file " + oss.str());
      return EXIT_FAILURE;
    }
  }

  // Write ThreeJS part file
  ThreeJS part;
  std::ostringstream oss;
  oss << threeJSPath << "/part.json";
  res = part.writePartFile(oss.str(), "result", 0, numberOfArrays, 0);
  if (!res) {
    Logger::ERROR("Unable to write ThreeJS part file " + oss.str());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}