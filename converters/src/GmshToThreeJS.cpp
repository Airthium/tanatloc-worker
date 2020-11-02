#include <algorithm>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

#include <sys/stat.h>
#include <sys/types.h>

#include "gmsh/Gmsh.hpp"
#include "logger/Logger.hpp"
#include "threeJS/ThreeJS.hpp"

Color generateColor();

/**
 * Main function
 */
int main(int argc, char *argv[]) {
  bool res;
  uint i;
  std::string meshFile;
  std::string threeJSPath;
  Gmsh *mesh = nullptr;

  // Arguments
  if (argc < 2) {
    Logger::ERROR("USAGE:");
    Logger::ERROR("./GmshToThreeJS meshFile [threeJSPath]");
    return EXIT_FAILURE;
  }
  meshFile = argv[1];

  if (argc >= 3) {
    threeJSPath = argv[2];
  } else {
    threeJSPath = meshFile;
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

  // Read & process mesh
  mesh = new Gmsh();
  res = mesh->load(meshFile);
  if (!res) {
    Logger::ERROR("Unable to load Gmsh file " + meshFile);
    return EXIT_FAILURE;
  }

  mesh->computeLabels();

  std::vector<double> *volumesVertices = mesh->getVolumesVertices();
  std::vector<double> *surfacesVertices = mesh->getSurfacesVertices();

  // Create & save ThreeJS
  // Volumes
  for (i = 0; i < mesh->getNumberOfTetrahedronLabels(); ++i) {
    // Vertices
    auto vertices = std::vector<float>();
    std::for_each(
        volumesVertices[i].begin(), volumesVertices[i].end(),
        [&vertices](const double &v) { vertices.push_back((float)v); });
    ThreeJS volume(vertices);

    // Color
    auto colors = std::vector<Color>();
    colors.push_back(generateColor());
    volume.setColors(colors);

    // Label
    volume.setLabel(mesh->getTetrahedronLabel(i));

    // Write
    std::ostringstream oss;
    oss << threeJSPath << "/" << SOLID << (i + 1) << ".json";
    res = volume.save(oss.str());
    if (!res) {
      Logger::ERROR("Unable to write ThreeJS file " + oss.str());
      return EXIT_FAILURE;
    }
  }

  // Surface
  for (i = 0; i < mesh->getNumberOfTriangleLabels(); ++i) {
    // Vertices
    auto vertices = std::vector<float>();
    std::for_each(
        surfacesVertices[i].begin(), surfacesVertices[i].end(),
        [&vertices](const double &v) { vertices.push_back((float)v); });
    ThreeJS surface(vertices);

    // Color
    auto colors = std::vector<Color>();
    colors.push_back(generateColor());
    surface.setColors(colors);

    // Label
    surface.setLabel(mesh->getTriangleLabel(i));

    // Write
    std::ostringstream oss;
    oss << threeJSPath << "/" << FACE << (i + 1) << ".json";
    res = surface.save(oss.str());
    if (!res) {
      Logger::ERROR("Unable to write ThreeJS file " + oss.str());
      return EXIT_FAILURE;
    }
  }

  // Write part file
  ThreeJS part;
  std::ostringstream oss;
  oss << threeJSPath << "/part.json";
  res = part.writePartFile(oss.str(), "mesh",
                           mesh->getNumberOfTetrahedronLabels(),
                           mesh->getNumberOfTriangleLabels());
  if (!res) {
    Logger::ERROR("Unable to write ThreeJS part file " + oss.str());
    return EXIT_FAILURE;
  }

  delete mesh;

  return EXIT_SUCCESS;
}

/**
 * Generate random
 */
double generateRandom() {
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_real_distribution<> dist(0., 1.);
  return dist(generator);
}

/**
 * Generate random color
 */
Color generateColor() {
  Color c;
  c.red = (float)generateRandom();
  c.green = (float)generateRandom();
  c.blue = (float)generateRandom();
  return c;
}
