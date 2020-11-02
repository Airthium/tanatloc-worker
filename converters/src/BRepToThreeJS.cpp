#include <iostream>
#include <sstream>
#include <string>

#include <sys/stat.h>
#include <sys/types.h>

#include <BRepTools.hxx>
#include <BRep_Builder.hxx>

#include "logger/Logger.hpp"
#include "occ/Triangulation.hpp"
#include "occ/getElements.hpp"
#include "threeJS/ThreeJS.hpp"

bool elementToThreeJS(const TopoDS_Shape, const std::string &, const uint);

/**
 * Main function
 */
int main(int argc, char *argv[]) {
  bool res;
  uint i;
  std::string brepFile;
  std::string threeJSPath;

  if (argc < 2) {
    Logger::ERROR("USAGE:");
    Logger::ERROR("./BRepToThreeJS brepFile [threeJSPath]");
    return EXIT_FAILURE;
  }
  brepFile = argv[1];

  if (argc >= 3) {
    threeJSPath = argv[2];
  } else {
    threeJSPath = brepFile;
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

  // Read BRep file
  TopoDS_Shape shape;
  BRep_Builder builder;
  res = BRepTools::Read(shape, brepFile.c_str(), builder);
  if (!res) {
    Logger::ERROR("Unable to read BRep file " + brepFile);
    return EXIT_FAILURE;
  }

  // Get solids and faces
  std::vector<TopoDS_Shape> solids = getSolids(shape);
  std::vector<TopoDS_Shape> faces;
  std::vector<TopoDS_Shape> edges;

  if (!solids.empty()) { // 3D
    faces = std::vector<TopoDS_Shape>();

    for (i = 0; i < solids.size(); ++i) {
      std::vector<TopoDS_Shape> temp = getFaces(solids[i]);
      std::copy(temp.begin(), temp.end(), back_inserter(faces));
    }
  } else { // 2D
    faces = getFaces(shape);

    for (i = 0; i < faces.size(); ++i) {
      std::vector<TopoDS_Shape> temp = getEdges(faces[i]);
      std::copy(temp.begin(), temp.end(), back_inserter(edges));
    }
  }

  // Save ThreeJS files
  // Solids
  for (i = 0; i < solids.size(); ++i) {
    res = elementToThreeJS(solids[i], threeJSPath + "/" + SOLID, i + 1);
    if (!res)
      return EXIT_FAILURE;

    Logger::DISP(std::to_string(0.5 * (i / (solids.size()))));
  }

  // Faces
  for (i = 0; i < faces.size(); ++i) {
    res = elementToThreeJS(faces[i], threeJSPath + "/" + FACE, i + 1);
    if (!res)
      return EXIT_FAILURE;

    Logger::DISP(std::to_string(0.5 + 0.5 * (i / ((double)faces.size() - 1.))));
  }

  // Edges
  for (i = 0; i < edges.size(); ++i) {
    Triangulation triangulation(edges[i]);
    triangulation.triangulate();
    std::vector<float> vertices = triangulation.getVertices();

    ThreeJS edge(vertices);

    edge.setLabel(i + 1);

    std::ostringstream oss;
    oss << threeJSPath << "/" << EDGE << (i + 1) << ".json";
    res = edge.save(oss.str());
    if (!res) {
      Logger::ERROR("Unable to write ThreeJS file " + oss.str());
      return EXIT_FAILURE;
    }
    Logger::DISP(std::to_string(0.5 + 0.5 * (i / ((double)edges.size() - 1.))));
  }

  // Write part file
  ThreeJS part;
  std::ostringstream oss;
  oss << threeJSPath << "/part.json";
  res = part.writePartFile(oss.str(), "geometry", (uint)solids.size(),
                           (uint)faces.size(), (uint)edges.size());
  if (!res) {
    Logger::ERROR("Unable to write ThreeJS part file " + oss.str());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

/**
 * OCC Element to ThreeJS
 * @param element Element
 * @param path Path
 * @param index Index
 */
bool elementToThreeJS(const TopoDS_Shape element, const std::string &path,
                      const uint index) {
  // Triangulation
  Triangulation triangulation(element);
  triangulation.triangulate();

  // Vertices, normals, indices
  std::vector<float> vertices = triangulation.getVertices();
  std::vector<float> normals = triangulation.getNormals();
  std::vector<uint> indices = triangulation.getIndices();

  // Part
  ThreeJS part(vertices, normals, indices);
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
