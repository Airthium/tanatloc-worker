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

    // Remove "," if any
    size_t pos = result.name.find_last_of(",");
    if (pos != std::string::npos)
      result.name.replace(pos, pos + 1, "");

    // Current path
    std::ostringstream oss;
    oss << threeJSPath << "_" << result.name;
    std::string currentPath = oss.str();

    // Create path
    int err = mkdir(currentPath.c_str(), S_IRWXU | S_IRWXG);
    if (err && errno != EEXIST) {
      Logger::ERROR("Unable to create threeJS path");
      return EXIT_FAILURE;
    }

    // Result as a face
    if (result.size == 1) { // Scalar
      ThreeJS face(result.vertices);
      face.setIndices(result.indices);
      face.setName(result.name);
      face.setData(result.values);

      oss.str("");
      oss.clear();
      oss << currentPath << "/" << FACE << 1 << ".json";
      if (!face.save(oss.str())) {
        Logger::ERROR("Unable to write ThreeJS file " + oss.str());
        return EXIT_FAILURE;
      }

      // Write ThreeJS part file
      ThreeJS part;
      oss.str("");
      oss.clear();
      oss << currentPath << "/part.json";
      res = part.writePartFile(oss.str(), "result", 0, 1, 0);
      if (!res) {
        Logger::ERROR("Unable to write ThreeJS part file " + oss.str());
        return EXIT_FAILURE;
      }
    } else if (result.size == 3) {
      // Magnitude
      RData magnitude = result;
      magnitude.size = 1;
      magnitude.name = result.name + " (magnitude)";

      auto magnitudeValues = std::vector<float>();
      for (int j = 0; j < magnitude.values.size() / 3; ++j) {
        float v = sqrt(pow(magnitude.values[3 * j + 0], 2) +
                       pow(magnitude.values[3 * j + 1], 2) +
                       pow(magnitude.values[3 * j + 2], 2));
        magnitudeValues.push_back(v);
      }

      magnitude.values = magnitudeValues;

      ThreeJS face(magnitude.vertices);
      face.setIndices(magnitude.indices);
      face.setName(magnitude.name);
      face.setData(magnitude.values);

      oss.str("");
      oss.clear();
      oss << currentPath << "/" << FACE << 1 << ".json";
      if (!face.save(oss.str())) {
        Logger::ERROR("Unable to write ThreeJS file " + oss.str());
        return EXIT_FAILURE;
      }

      // Component 1, 2 & 3
      for (int k = 0; k < 3; ++k) {
        RData component = result;
        component.size = 1;
        oss.str("");
        oss.clear();
        oss << result.name << " (component " << k + 1 << ")";
        component.name = oss.str();

        auto componentValues = std::vector<float>();
        for (int j = 0; j < component.values.size() / 3; ++j) {
          float v = component.values[3 * j + k];
          componentValues.push_back(v);
        }

        component.values = componentValues;

        ThreeJS face(component.vertices);
        face.setIndices(component.indices);
        face.setName(component.name);
        face.setData(component.values);

        oss.str("");
        oss.clear();
        oss << currentPath << "/" << FACE << k + 2 << ".json";
        if (!face.save(oss.str())) {
          Logger::ERROR("Unable to write ThreeJS file " + oss.str());
          return EXIT_FAILURE;
        }
      }

      // Write ThreeJS part file
      ThreeJS part;
      oss.str("");
      oss.clear();
      oss << currentPath << "/part.json";
      res = part.writePartFile(oss.str(), "result", 0, 4, 0);
      if (!res) {
        Logger::ERROR("Unable to write ThreeJS part file " + oss.str());
        return EXIT_FAILURE;
      }
    }

    Logger::DISP("{ \"path\": \"" + threeJSPath + "_" + result.name +
                 "\", \"name\": \"" + result.name + "\"}");
  }

  return EXIT_SUCCESS;
}