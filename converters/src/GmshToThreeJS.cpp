#include <iostream>
#include <random>
#include <sstream>
#include <string>

#include <sys/stat.h>
#include <sys/types.h>

#include "gmsh/Gmsh.hpp"
#include "logger/Logger.hpp"
#include "threeJS/ThreeJS.hpp"

void generateColor(float *, float *, float *);

/**
 * Main function
 */
int main(int argc, char *argv[]) {
  bool res;
  uint i;
  std::string meshFile;
  std::string threeJSPath;
  Gmsh *mesh = nullptr;

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
  for (i = 0; i < mesh->getNumberOfTetrahedronLabels(); ++i) {
    double *vertices = &volumesVertices[i][0];
    uint size = (uint)volumesVertices[i].size();
    ThreeJS volume(vertices, size);
    auto **colors = new float *[1];
    float r;
    float g;
    float b;
    generateColor(&r, &g, &b);
    colors[0] = new float[3];
    colors[0][0] = r;
    colors[0][1] = g;
    colors[0][2] = b;
    volume.setColors(colors, 1);
    volume.setLabel(mesh->getTetrahedronLabel(i));

    std::ostringstream oss;
    oss << threeJSPath << "/" << SOLID << (i + 1) << ".json";
    res = volume.save(oss.str());
    if (!res) {
      Logger::ERROR("Unable to write ThreeJS file " + oss.str());
      return EXIT_FAILURE;
    }
  }

  for (i = 0; i < mesh->getNumberOfTriangleLabels(); ++i) {
    double *vertices = &surfacesVertices[i][0];
    uint size = (uint)surfacesVertices[i].size();
    ThreeJS surface(vertices, size);
    float **colors = new float *[1];
    float r;
    float g;
    float b;
    generateColor(&r, &g, &b);
    colors[0] = new float[3];
    colors[0][0] = r;
    colors[0][1] = g;
    colors[0][2] = b;
    surface.setColors(colors, 1);
    surface.setLabel(mesh->getTriangleLabel(i));
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
void generateColor(float *r, float *g, float *b) {
  *r = (float)generateRandom();
  *g = (float)generateRandom();
  *b = (float)generateRandom();
}
