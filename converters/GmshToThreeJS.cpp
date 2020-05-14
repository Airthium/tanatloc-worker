#include <iostream>
#include <random>
#include <sstream>
#include <string>

#include <sys/stat.h>
#include <sys/types.h>

#include "gmsh/Gmsh.hpp"
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
    std::cerr << "USAGE:" << std::endl;
    std::cerr << "./GmshToThreeJS meshFile [threeJSPath]" << std::endl;
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
  int err = mkdir(threeJSPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  if (err && errno != EEXIST) {
    std::cerr << "Unable to create threeJS path" << std::endl;
    return EXIT_FAILURE;
  }

  // Read & process mesh
  mesh = new Gmsh();
  res = mesh->load(meshFile);
  if (!res) {
    std::cerr << "Unable to load Gmsh file " << meshFile << std::endl;
    return EXIT_FAILURE;
  }

  mesh->computeLabels();

  std::vector<double> *volumesVertices = mesh->getVolumesVertices();
  std::vector<double> *surfacesVertices = mesh->getSurfacesVertices();

  // Create & save ThreeJS
  for (i = 0; i < mesh->getNumberOfTetrahedronLabels(); ++i) {
    double *vertices = &volumesVertices[i][0];
    uint size = volumesVertices[i].size();
    ThreeJS volume(vertices, size);
    float **colors = new float *[1];
    float r, g, b;
    generateColor(&r, &g, &b);
    colors[0] = new float[3];
    colors[0][0] = r;
    colors[0][1] = g;
    colors[0][2] = b;
    volume.setColors(colors, 1);
    volume.setLabel(mesh->getTetrahedronLabel(i));

    std::ostringstream oss;
    oss << threeJSPath << "/" << SOLID << i << ".json";
    res = volume.save(oss.str());
    if (!res) {
      std::cerr << "Unable to write ThreeJS file " << oss.str() << std::endl;
      return EXIT_FAILURE;
    }
  }

  for (i = 0; i < mesh->getNumberOfTriangleLabels(); ++i) {
    double *vertices = &surfacesVertices[i][0];
    uint size = surfacesVertices[i].size();
    ThreeJS surface(vertices, size);
    float **colors = new float *[1];
    float r, g, b;
    generateColor(&r, &g, &b);
    colors[0] = new float[3];
    colors[0][0] = r;
    colors[0][1] = g;
    colors[0][2] = b;
    surface.setColors(colors, 1);
    surface.setLabel(mesh->getTriangleLabel(i));
    std::ostringstream oss;
    oss << threeJSPath << "/" << FACE << i << ".json";
    res = surface.save(oss.str());
    if (!res) {
      std::cerr << "Unable to write ThreeJS file " << oss.str() << std::endl;
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
    std::cerr << "Unable to write ThreeJS part file " << oss.str() << std::endl;
    return EXIT_FAILURE;
  }

  delete mesh;

  return EXIT_SUCCESS;
}
/**
 * Generate random color
 */
void generateColor(float *r, float *g, float *b) {
  {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<> dist(0., 1.);
    *r = dist(generator);
  }
  {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<> dist(0., 1.);
    *g = dist(generator);
  }
  {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<> dist(0., 1.);
    *b = dist(generator);
  }
}
