#include <algorithm>
#include <random>

#include "gmsh/Gmsh.hpp"
#include "logger/Logger.hpp"
#include "occ/GLTFWriter.hpp"
#include "occ/MainDocument.hpp"
#include "occ/Triangulation.hpp"
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <Quantity_Color.hxx>
#include <TopoDS_Builder.hxx>
#include <gp_Pnt.hxx>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "tiny_gltf.h"

std::vector<double> generateColor();

/**
 * Main function
 */
int main(int argc, char *argv[]) {
  bool res;
  uint i;
  std::string meshFile;
  std::string gltfFile;

  // Arguments
  if (argc < 3) {
    Logger::ERROR("USAGE:");
    Logger::ERROR("GmshToGLTF meshFile gltfFile");
    return EXIT_FAILURE;
  }
  meshFile = argv[1];
  gltfFile = argv[2];

  // Read & process mesh
  auto gmsh = std::make_unique<Gmsh>();
  res = gmsh->load(meshFile);
  if (!res) {
    Logger::ERROR("Unable to load Gmsh file " + meshFile);
    return EXIT_FAILURE;
  }

  // Compute labels
  gmsh->computeLabels();

  {
    // GLTF
    tinygltf::Model model;
    tinygltf::Scene scene;
    tinygltf::Asset asset;

    // std::vector<Vertex> *volumesVertices = gmsh->getVolumesVertices();
    std::vector<Vertex> *surfacesVertices = gmsh->getSurfacesVertices();

    for (uint i = 0; i < gmsh->getNumberOfTriangleLabels(); ++i) {
      tinygltf::Node node;
      tinygltf::Mesh mesh;
      tinygltf::Buffer buffer;
      tinygltf::BufferView bufferView;
      tinygltf::Accessor accessor;
      tinygltf::Primitive primitive;
      tinygltf::Material material;

      auto trianglesVertices = surfacesVertices[i];

      // Vertices
      std::vector<float> vertices;
      for (size_t j = 0; j < trianglesVertices.size(); ++j) {
        Vertex vertex = trianglesVertices.at(j);
        vertices.push_back(vertex.X());
        vertices.push_back(vertex.Y());
        vertices.push_back(vertex.Z());
      }

      // Buffer
      for (size_t j = 0; j < vertices.size(); ++j) {
        float vertex = vertices.at(j);
        unsigned char buf[__SIZEOF_FLOAT__];
        std::memcpy(buf, &vertex, __SIZEOF_FLOAT__);

        for (size_t k = 0; k < __SIZEOF_FLOAT__; ++k) {
          buffer.data.push_back(buf[k]);
        }
      }
      model.buffers.push_back(buffer);

      // Buffer view
      bufferView.buffer = model.buffers.size() - 1;
      bufferView.byteOffset = 0;
      bufferView.byteLength = vertices.size() * __SIZEOF_FLOAT__;
      bufferView.target = TINYGLTF_TARGET_ARRAY_BUFFER;
      model.bufferViews.push_back(bufferView);

      // Accessor
      accessor.bufferView = model.bufferViews.size() - 1;
      accessor.byteOffset = 0;
      accessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
      accessor.count = vertices.size() / 3;
      accessor.type = TINYGLTF_TYPE_VEC3;
      accessor.maxValues = {1.0, 1.0, 1.0};
      accessor.minValues = {0.0, 0.0, 0.0}; // TODO min/max
      model.accessors.push_back(accessor);

      // Material
      material.pbrMetallicRoughness.baseColorFactor = generateColor();
      material.doubleSided = true;
      model.materials.push_back(material);

      // Primitive
      primitive.attributes["POSITION"] = model.accessors.size() - 1;
      primitive.material = model.materials.size() - 1;
      primitive.mode = TINYGLTF_MODE_TRIANGLES;

      // Mesh
      mesh.name = "Face " + std::to_string(i);
      // mesh.extras = tinygltf::Value() // TODO set userData
      mesh.primitives.push_back(primitive);
      model.meshes.push_back(mesh);

      // Node
      node.mesh = model.meshes.size() - 1;
      model.nodes.push_back(node);

      // Scene
      scene.nodes.push_back(model.nodes.size() - 1);
    }

    // Scenes
    model.scenes.push_back(scene);

    // Asset
    asset.version = "2.0";
    asset.generator = "tanatloc-converter";
    model.asset = asset;

    // Save
    tinygltf::TinyGLTF gltf;
    res = gltf.WriteGltfSceneToFile(&model, gltfFile,
                                    true,  // embedImages
                                    true,  // embedBuffers
                                    true,  // pretty print
                                    true); // write binary
    if (!res) {
      Logger::ERROR("Unable to write glft file " + gltfFile);
      return EXIT_FAILURE;
    }
  }

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
std::vector<double> generateColor() {
  return {generateRandom(), generateRandom(), generateRandom(), 1.0f};
}
