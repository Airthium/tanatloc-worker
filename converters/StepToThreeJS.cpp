#include <iostream>
#include <sstream>
#include <string>

#include <sys/stat.h>
#include <sys/types.h>

#include "occ/StepReader.hpp"
#include "occ/Triangulation.hpp"
#include "occ/getElements.hpp"
#include "threeJS/ThreeJS.hpp"

#define DEFAULT_SOLID_COLOR_R 0.75
#define DEFAULT_SOLID_COLOR_G 0.75
#define DEFAULT_SOLID_COLOR_B 0.75
#define DEFAULT_FACE_COLOR_R 0.75
#define DEFAULT_FACE_COLOR_G 0.75
#define DEFAULT_FACE_COLOR_B 0.75

/**
 * Main function
 */
int main(int argc, char *argv[]) {
  bool res;
  uint i, j;
  std::string stepFile;
  std::string threeJSPath;

  if (argc < 2) {
    std::cerr << "USAGE:" << std::endl;
    std::cerr << "./StepToThreeJS stepFile [threeJSPath]" << std::endl;
    return EXIT_FAILURE;
  }
  stepFile = argv[1];

  if (argc >= 3) {
    threeJSPath = argv[2];
  } else {
    threeJSPath = stepFile;
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

  // Read step file
  StepReader reader = StepReader(stepFile);
  res = reader.read();
  if (!res) {
    std::cerr << "Unable to load step file " << stepFile << std::endl;
    return EXIT_FAILURE;
  }
  std::vector<TopoDS_Shape> shapes = reader.getShapes();
  Handle(TDocStd_Document) document = reader.getDocument();

  // Get solids and faces
  std::vector<std::pair<bool, Quantity_Color>> solidColors;
  std::vector<TopoDS_Shape> solids = std::vector<TopoDS_Shape>();
  std::vector<std::pair<bool, Quantity_Color>> faceColors;
  std::vector<TopoDS_Shape> faces = std::vector<TopoDS_Shape>();

  for (i = 0; i < shapes.size(); ++i) {
    std::vector<TopoDS_Shape> solidTemp =
        getSolids(shapes[i], document, &solidColors);
    std::copy(solidTemp.begin(), solidTemp.end(), back_inserter(solids));
    for (j = 0; j < solidTemp.size(); ++j) {
      std::vector<std::pair<bool, Quantity_Color>> tempColors;
      std::vector<TopoDS_Shape> faceTemp =
          getFaces(solidTemp[j], document, &tempColors);
      std::copy(faceTemp.begin(), faceTemp.end(), back_inserter(faces));
      std::copy(tempColors.begin(), tempColors.end(),
                back_inserter(faceColors));
    }
  }

  // Save ThreeJS files
  for (i = 0; i < solids.size(); ++i) {
    Triangulation triangulation(solids[i]);
    triangulation.triangulate();
    std::vector<float> vertices = triangulation.getVertices();
    std::vector<float> normals = triangulation.getNormals();
    std::vector<uint> indices = triangulation.getIndices();

    float **colors = new float *[0];
    colors[0] = new float[3];
    colors[0][0] = std::get<0>(solidColors[i])
                       ? std::get<1>(solidColors[i]).Red()
                       : DEFAULT_SOLID_COLOR_R;
    colors[0][1] = std::get<0>(solidColors[i])
                       ? std::get<1>(solidColors[i]).Green()
                       : DEFAULT_SOLID_COLOR_G;
    colors[0][2] = std::get<0>(solidColors[i])
                       ? std::get<1>(solidColors[i]).Blue()
                       : DEFAULT_SOLID_COLOR_B;

    ThreeJS solid(&vertices[0], vertices.size(), &normals[0], normals.size(),
                  &indices[0], indices.size());
    solid.setColors(colors, 1);
    solid.setLabel(i + 1);
    double min, max;
    triangulation.getBb(&min, &max);
    solid.setMinMax(min, max);
    std::ostringstream oss;
    oss << threeJSPath << "/" << SOLID << (i + 1) << ".json";
    res = solid.save(oss.str());
    if (!res) {
      std::cerr << "Unable to write ThreeJS file " << oss.str() << std::endl;
      return EXIT_FAILURE;
    }
    // std::cout << 0.5 * (i / (solids.size())) << std::endl;
  }

  for (i = 0; i < faces.size(); ++i) {
    Triangulation triangulation(faces[i]);
    triangulation.triangulate();
    std::vector<float> vertices = triangulation.getVertices();
    std::vector<float> normals = triangulation.getNormals();
    std::vector<uint> indices = triangulation.getIndices();

    float **colors = new float *[0];
    colors[0] = new float[3];
    colors[0][0] = std::get<0>(faceColors[i]) ? std::get<1>(faceColors[i]).Red()
                                              : DEFAULT_FACE_COLOR_R;
    colors[0][1] = std::get<0>(faceColors[i])
                       ? std::get<1>(faceColors[i]).Green()
                       : DEFAULT_FACE_COLOR_G;
    colors[0][2] = std::get<0>(faceColors[i])
                       ? std::get<1>(faceColors[i]).Blue()
                       : DEFAULT_FACE_COLOR_B;

    ThreeJS face(&vertices[0], vertices.size(), &normals[0], normals.size(),
                 &indices[0], indices.size());
    face.setColors(colors, 1);
    face.setLabel(i + 1);
    std::ostringstream oss;
    oss << threeJSPath << "/" << FACE << (i + 1) << ".json";
    res = face.save(oss.str());
    if (!res) {
      std::cerr << "Unable to write ThreeJS file " << oss.str() << std::endl;
      return EXIT_FAILURE;
    }
    // std::cout << 0.5 + 0.5 * (i / (faces.size() - 1.)) << std::endl;
  }

  // Write part file
  ThreeJS part;
  std::ostringstream oss;
  oss << threeJSPath << "/part.json";
  res = part.writePartFile(oss.str(), "geometry", solids.size(), faces.size());
  if (!res) {
    std::cerr << "Unable to write ThreeJS part file " << oss.str() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
