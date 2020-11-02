#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include <sys/stat.h>
#include <sys/types.h>

#include "logger/Logger.hpp"
#include "occ/StepReader.hpp"
#include "occ/Triangulation.hpp"
#include "occ/getElements.hpp"
#include "threeJS/ThreeJS.hpp"

// Default color
constexpr Color DEFAULT_COLOR = {0.75, 0.75, 0.75};

bool elementToThreeJS(const TopoDS_Shape, const std::pair<bool, Quantity_Color>,
                      const std::string &, const uint);

/**
 * Main function
 */
int main(int argc, char *argv[]) {
  bool res;
  uint i;
  std::string stepFile;
  std::string threeJSPath;

  // Arguments
  if (argc < 2) {
    Logger::ERROR("USAGE:");
    Logger::ERROR("./StepToThreeJS stepFile [threeJSPath]");
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
  int err = mkdir(threeJSPath.c_str(), S_IRWXU | S_IRWXG);
  if (err && errno != EEXIST) {
    Logger::ERROR("Unable to create threeJS path");
    return EXIT_FAILURE;
  }

  // Read step file
  auto reader = StepReader(stepFile);
  res = reader.read();
  if (!res) {
    Logger::ERROR("Unable to load step file " + stepFile);
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
    std::for_each(solidTemp.begin(), solidTemp.end(),
                  [document, &faceColors, &faces](const TopoDS_Shape &solid) {
                    std::vector<std::pair<bool, Quantity_Color>> tempColors;
                    std::vector<TopoDS_Shape> faceTemp =
                        getFaces(solid, document, &tempColors);
                    std::copy(faceTemp.begin(), faceTemp.end(),
                              std::back_inserter(faces));
                    std::copy(tempColors.begin(), tempColors.end(),
                              std::back_inserter(faceColors));
                  });
  }

  // Save ThreeJS files
  // Solids
  for (i = 0; i < solids.size(); ++i) {
    res = elementToThreeJS(solids[i], solidColors[i], threeJSPath + "/" + SOLID,
                           i + 1);
    if (!res)
      return EXIT_FAILURE;

    Logger::DISP(std::to_string(0.5 * (i / (solids.size()))));
  }

  // Faces
  for (i = 0; i < faces.size(); ++i) {
    res = elementToThreeJS(faces[i], faceColors[i], threeJSPath + "/" + FACE,
                           i + 1);
    if (!res)
      return EXIT_FAILURE;

    Logger::DISP(std::to_string(0.5 + 0.5 * (i / ((double)faces.size() - 1.))));
  }

  // Write part file
  ThreeJS part;
  std::ostringstream oss;
  oss << threeJSPath << "/part.json";
  res = part.writePartFile(oss.str(), "geometry", (uint)solids.size(),
                           (uint)faces.size());
  if (!res) {
    Logger::ERROR("Unable to write ThreeJS part file " + oss.str());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

/**
 * OCC element to ThreeJS
 * @param element Element
 * @param color Color
 * @param path Path
 * @param index Index
 */
bool elementToThreeJS(const TopoDS_Shape element,
                      const std::pair<bool, Quantity_Color> color,
                      const std::string &path, const uint index) {
  // Triangulation
  Triangulation triangulation(element);
  triangulation.triangulate();

  // Vertices, normals, indices
  std::vector<float> vertices = triangulation.getVertices();
  std::vector<float> normals = triangulation.getNormals();
  std::vector<uint> indices = triangulation.getIndices();

  // Color
  std::vector<Color> colors;
  Color c;
  c.red =
      std::get<0>(color) ? (float)std::get<1>(color).Red() : DEFAULT_COLOR.red;
  c.green = std::get<0>(color) ? (float)std::get<1>(color).Green()
                               : DEFAULT_COLOR.green;
  c.blue = std::get<0>(color) ? (float)std::get<1>(color).Blue()
                              : DEFAULT_COLOR.blue;
  colors.push_back(c);

  // Part
  ThreeJS part(vertices, normals, indices);
  part.setColors(colors);
  part.setLabel(index);

  // Write
  std::ostringstream oss;
  oss << path << index << ".json";
  if (!part.save(oss.str())) {
    Logger::ERROR("Unable to write ThreeJS file " + oss.str());
    return false;
  }

  return true;
}