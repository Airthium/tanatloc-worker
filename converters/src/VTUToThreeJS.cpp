#include <sstream>

#include <sys/stat.h>
#include <sys/types.h>

#include "logger/Logger.hpp"
#include "threeJS/ThreeJS.hpp"
#include "vtk/VTUReader.hpp"

RData getMagnitude(const RData &);
RData getComponent(const RData &, const int);
bool writeOne(const RData &, const std::string &);

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

  // Read VTU file
  auto reader = VTUReader(vtuFile);
  res = reader.read();
  if (!res) {
    Logger::ERROR("Unable to read VTU file " + vtuFile);
    return EXIT_FAILURE;
  }
  std::vector<RData> arrays = reader.getArrays();

  // Write ThreeJS files
  const auto numberOfArrays = (int)arrays.size();
  for (int i = 0; i < numberOfArrays; ++i) {
    RData result = arrays[i];

    if (result.size == 1) { // Scalar
      if (!writeOne(result, threeJSPath + "_" + result.name))
        return EXIT_FAILURE;
    } else if (result.size == 3) {
      // Magnitude
      RData magnitude = getMagnitude(result);
      if (!writeOne(magnitude, threeJSPath + "_" + result.name + "_magnitude"))
        return EXIT_FAILURE;

      // Component 1, 2 & 3
      for (int j = 0; j < 3; ++j) {
        RData component = getComponent(result, j);
        if (!writeOne(component, threeJSPath + "_" + result.name +
                                     "_component" + std::to_string(j + 1)))
          return EXIT_FAILURE;
      }
    }
  }

  return EXIT_SUCCESS;
}

RData getMagnitude(const RData &result) {
  RData magnitude = result;
  magnitude.size = 1;
  magnitude.name = result.name + " (magnitude)";

  auto magnitudeValues = std::vector<float>();
  for (int i = 0; i < (int)magnitude.values.size() / 3; ++i) {
    auto v = (float)sqrt(pow(magnitude.values[3 * i + 0], 2) +
                         pow(magnitude.values[3 * i + 1], 2) +
                         pow(magnitude.values[3 * i + 2], 2));
    magnitudeValues.push_back(v);
  }

  magnitude.values = magnitudeValues;

  return magnitude;
}

RData getComponent(const RData &result, const int index) {
  RData component;
  component.size = 1;
  component.name =
      result.name + " (component " + std::to_string(index + 1) + ")";

  auto componentValues = std::vector<float>();
  for (int i = 0; i < (int)component.values.size() / 3; ++i) {
    float v = component.values[3 * i + index];
    componentValues.push_back(v);
  }

  component.values = componentValues;

  return component;
}

bool writeOne(const RData &result, const std::string &path) {
  // Create path
  int err = mkdir(path.c_str(), S_IRWXU | S_IRWXG);
  if (err && errno != EEXIST) {
    Logger::ERROR("Unable to create threeJS path");
    return false;
  }

  // Result as a face
  ThreeJS face(result.vertices);
  face.setIndices(result.indices);
  face.setName(result.name);
  face.setData(result.values);

  std::string faceFile = path + "/" + FACE + std::to_string(1) + ".json";
  if (!face.save(faceFile)) {
    Logger::ERROR("Unable to write ThreeJS file " + faceFile);
    return false;
  }

  // Write ThreeJS part file
  ThreeJS part;
  std::string partFile = path + "/part.json";
  bool res = part.writePartFile(partFile, "result", 0, 1, 0);
  if (!res) {
    Logger::ERROR("Unable to write ThreeJS part file " + partFile);
    return false;
  }

  Logger::DISP("{ \"path\": \"" + path + "\", \"name\": \"" + result.name +
               "\"}");

  return true;
}