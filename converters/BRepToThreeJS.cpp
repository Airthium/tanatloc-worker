#include <iostream>
#include <sstream>
#include <string>

#include <sys/stat.h>
#include <sys/types.h>

#include <BRepTools.hxx>
#include <BRep_Builder.hxx>

#include "occ/Triangulation.hpp"
#include "occ/getElements.hpp"
#include "threeJS/ThreeJS.hpp"

/**
 * Main function
 */
int main(int argc, char *argv[]) {
  bool res;
  uint i;
  std::string brepFile;
  std::string threeJSPath;

  if (argc < 2) {
    std::cerr << "USAGE:" << std::endl;
    std::cerr << "./BRepToThreeJS brepFile [threeJSPath]" << std::endl;
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
  int err = mkdir(threeJSPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  if (err && errno != EEXIST) {
    std::cerr << "Unable to create threeJS path" << std::endl;
    return EXIT_FAILURE;
  }

  // Read BRep file
  TopoDS_Shape shape;
  BRep_Builder builder;
  res = BRepTools::Read(shape, brepFile.c_str(), builder);
  if (!res) {
    std::cerr << "Unable to read BRep file " << brepFile << std::endl;
    return EXIT_FAILURE;
  }

  // Get solids and faces
  std::vector<TopoDS_Shape> solids = getSolids(shape);
  std::vector<TopoDS_Shape> faces;
  std::vector<TopoDS_Shape> edges;

  if (solids.size() > 0) { // 3D
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
  for (i = 0; i < solids.size(); ++i) {
    Triangulation triangulation(solids[i]);
    triangulation.triangulate();
    std::vector<float> vertices = triangulation.getVertices();
    std::vector<float> normals = triangulation.getNormals();
    std::vector<uint> indices = triangulation.getIndices();

    ThreeJS solid(&vertices[0], vertices.size(), &normals[0], normals.size(),
                  &indices[0], indices.size());

    solid.setLabel(i + 1);

    double min, max;
    triangulation.getBb(&min, &max);
    solid.setMinMax(min, max);

    std::ostringstream oss;
    oss << threeJSPath << "/" << SOLID << (i+1) << ".json";
    res = solid.save(oss.str());
    if (!res) {
      std::cerr << "Unable to write ThreeJS file " << oss.str() << std::endl;
      return EXIT_FAILURE;
    }
    std::cout << 0.5 * (i / (solids.size())) << std::endl;
  }

  for (i = 0; i < faces.size(); ++i) {
    Triangulation triangulation(faces[i]);
    triangulation.triangulate();
    std::vector<float> vertices = triangulation.getVertices();
    std::vector<float> normals = triangulation.getNormals();
    std::vector<uint> indices = triangulation.getIndices();

    ThreeJS face(&vertices[0], vertices.size(), &normals[0], normals.size(),
                 &indices[0], indices.size());

    face.setLabel(i + 1);

    std::ostringstream oss;
    oss << threeJSPath << "/" << FACE << (i+1) << ".json";
    res = face.save(oss.str());
    if (!res) {
      std::cerr << "Unable to write ThreeJS file " << oss.str() << std::endl;
      return EXIT_FAILURE;
    }
    std::cout << 0.5 + 0.5 * (i / (faces.size() - 1.)) << std::endl;
  }

  for (i = 0; i < edges.size(); ++i) {
    Triangulation triangulation(edges[i]);
    triangulation.triangulate();
    std::vector<float> vertices = triangulation.getVertices();

    ThreeJS edge(&vertices[0], vertices.size());

    edge.setLabel(i + 1);

    std::ostringstream oss;
    oss << threeJSPath << "/" << EDGE << (i+1) << ".json";
    res = edge.save(oss.str());
    if (!res) {
      std::cerr << "Unable to write ThreeJS file " << oss.str() << std::endl;
      return EXIT_FAILURE;
    }
    std::cout << 0.5 + 0.5 * (i / (edges.size() - 1.)) << std::endl;
  }

  // Write part file
  ThreeJS part;
  std::ostringstream oss;
  oss << threeJSPath << "/part.json";
  res = part.writePartFile(oss.str(), "geometry", solids.size(), faces.size(),
                           edges.size());
  if (!res) {
    std::cerr << "Unable to write ThreeJS part file " << oss.str() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
