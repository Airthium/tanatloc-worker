#include "ThreeJS.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>

#include "../logger/Logger.hpp"

/**
 * Constructor
 */
ThreeJS::ThreeJS() {}

/**
 * Constructor
 * @param vertices Vertices (float)
 */
ThreeJS::ThreeJS(const std::vector<float> &vertices) : m_vertices(vertices) {}

/**
 * Constructor
 * @param vertices Vertices
 * @param normals Normals
 */
ThreeJS::ThreeJS(const std::vector<float> &vertices,
                 const std::vector<float> &normals)
    : m_vertices(vertices), m_normals(normals) {}

/**
 * Constructor
 * @param vertices Vertices
 * @param normals Normals
 * @param indices Indices
 */
ThreeJS::ThreeJS(const std::vector<float> &vertices,
                 const std::vector<float> &normals,
                 const std::vector<uint> &indices)
    : m_vertices(vertices), m_normals(normals), m_indices(indices) {}

/**
 * Set name
 * @param name Name
 */
void ThreeJS::setName(const std::string &name) { this->m_name = name; }

/**
 * Set label
 * @param number Label
 */
void ThreeJS::setLabel(uint label) { this->m_label = label; }

/**
 * Set vertices
 * @param vertices Vertices
 */
void ThreeJS::setVertices(const std::vector<float> &vertices) {
  if (this->m_vertices.size())
    this->m_vertices.clear();

  this->m_vertices = vertices;
}

/**
 * Set normals
 * @param normals Normals
 */
void ThreeJS::setNormals(const std::vector<float> &normals) {
  if (this->m_normals.size())
    this->m_normals.clear();

  this->m_normals = normals;
}

/**
 * Set indices
 * @param indices Indices
 */
void ThreeJS::setIndices(const std::vector<uint> &indices) {
  if (this->m_indices.size())
    this->m_indices.clear();

  this->m_indices = indices;
}

/**
 * Set color
 * @param colors Colors
 */
void ThreeJS::setColors(const std::vector<Color> &colors) {
  if (this->m_colors.size())
    this->m_colors.clear();

  this->m_colors = colors;
}

/**
 * Set data
 * @param data Data
 */
void ThreeJS::setData(const std::vector<float> &data) {
  if (this->m_data.size())
    this->m_data.clear();

  this->m_data = data;
}

/**
 * Saver
 * @param {string} fileName File name
 * @returns {boolean} Save status
 */
bool ThreeJS::save(const std::string &fileName) const {
  std::ofstream file(fileName, std::ios::out | std::ios::trunc);
  if (!file) {
    Logger::ERROR("Unable to open " + fileName);
    return false;
  }

  // Headers
  this->saveHeader(file);

  // Vertices
  if (this->m_vertices.size())
    this->saveVertices(file);

  // Normals
  if (this->m_normals.size())
    this->saveNormals(file);

  // Colors
  if (this->m_colors.size())
    this->saveColors(file);

  // Data
  if (this->m_data.size())
    this->saveData(file);

  // Footer
  this->saveFooter(file);

  return true;
}

/**
 * Save header
 */
void ThreeJS::saveHeader(std::ofstream &file) const {
  file << "{" << std::endl;
  file << "\t\"metadata\": {" << std::endl;
  file << "\t\t\"version\": 4.4," << std::endl;
  file << "\t\t\"type\": \"BufferGeometry\"," << std::endl;
  file << "\t\t\"generator\": \"Tanatloc\"" << std::endl;
  file << "\t}," << std::endl;
  file << "\t\"uuid\": \"" << this->generateUUID() << "\"," << std::endl;
  if (this->m_name != "")
    file << "\t\"name\": \"" << this->m_name << "\"," << std::endl;
  if (this->m_label)
    file << "\t\"label\": \"" << this->m_label << "\"," << std::endl;
  file << "\t\"type\": \"BufferGeometry\"," << std::endl;
  file << "\t\"data\": {" << std::endl;
  file << "\t\t\"attributes\": {" << std::endl;
}

/**
 * Save vertices
 */
void ThreeJS::saveVertices(std::ofstream &file) const {
  uint i;

  file << "\t\t\t\"position\": {" << std::endl;
  file << "\t\t\t\t\"itemSize\": " << this->m_dimension << "," << std::endl;
  file << "\t\t\t\t\"type\": \"Float32Array\"," << std::endl;
  file << "\t\t\t\t\"array\": [";

  const auto numberOfIndices = this->m_indices.size();
  if (numberOfIndices) {
    // With indices
    for (i = 0; i < numberOfIndices; ++i) {
      file << this->m_vertices[3 * this->m_indices[i] + 0] << ","
           << this->m_vertices[3 * this->m_indices[i] + 1] << ","
           << this->m_vertices[3 * this->m_indices[i] + 2];
      if (i < (numberOfIndices - 1))
        file << ",";
    }
  } else {
    // Without indices
    const auto numberOfVertices = this->m_vertices.size();
    for (i = 0; i < numberOfVertices; ++i) {
      file << this->m_vertices[i];
      if (i < (numberOfVertices - 1))
        file << ",";
    }
  }

  file << "]" << std::endl;
  file << "\t\t\t}";

  if (this->m_normals.size() || this->m_colors.size() || this->m_data.size())
    file << "," << std::endl;
  else
    file << std::endl;
}

/**
 * Save normals
 */
void ThreeJS::saveNormals(std::ofstream &file) const {
  uint i;

  file << "\t\t\t\"normal\": {" << std::endl;
  file << "\t\t\t\t\"itemSize\": " << this->m_dimension << "," << std::endl;
  file << "\t\t\t\t\"type\": \"Float32Array\"," << std::endl;
  file << "\t\t\t\t\"array\": [";

  const auto numberOfIndices = this->m_indices.size();
  if (numberOfIndices) {
    // With indices
    for (i = 0; i < numberOfIndices; ++i) {
      file << this->m_normals[3 * this->m_indices[i] + 0] << ","
           << this->m_normals[3 * this->m_indices[i] + 1] << ","
           << this->m_normals[3 * this->m_indices[i] + 2];
      if (i < (numberOfIndices - 1))
        file << ",";
    }
  } else {
    // Without indices
    const auto numberOfNormals = this->m_normals.size();
    for (i = 0; i < numberOfNormals; ++i) {
      file << this->m_normals[i];
      if (i < (numberOfNormals - 1))
        file << ",";
    }
  }

  file << "]" << std::endl;
  file << "\t\t\t}";

  if (this->m_colors.size() || this->m_data.size())
    file << "," << std::endl;
  else
    file << std::endl;
}

/**
 * Save colors
 */
void ThreeJS::saveColors(std::ofstream &file) const {
  file << "\t\t\t\"color\": {" << std::endl;
  file << "\t\t\t\t\"itemSize\": " << this->m_dimension << "," << std::endl;
  file << "\t\t\t\t\"type\": \"Float32Array\"," << std::endl;
  file << "\t\t\t\t\"array\": [";

  const auto numberOfColors = this->m_colors.size();
  for (uint i = 0; i < numberOfColors; ++i) {
    file << this->m_colors[i].red << "," << this->m_colors[i].green << ","
         << this->m_colors[i].blue;
    if (i < (numberOfColors - 1))
      file << ",";
  }

  file << "]" << std::endl;
  file << "\t\t\t}" << std::endl;

  if (this->m_data.size())
    file << "," << std::endl;
  else
    file << std::endl;
}

/**
 * Save data
 */
void ThreeJS::saveData(std::ofstream &file) const {
  uint i;
  const auto numberOfData = this->m_data.size();

  int itemSize = numberOfData == this->m_vertices.size() ? 3 : 1;

  file << "\t\t\t\"data\": {" << std::endl;
  file << "\t\t\t\t\"itemSize\": " << itemSize << "," << std::endl;
  file << "\t\t\t\t\"type\": \"Float32Array\"," << std::endl;
  file << "\t\t\t\t\"array\": [";

  const auto numberOfIndices = this->m_indices.size();
  if (numberOfIndices) {
    // With indices
    for (i = 0; i < numberOfIndices; ++i) {
      file << this->m_data[this->m_indices[i]];
      if (i < (numberOfIndices - 1))
        file << ",";
    }
  } else {
    // Without indices
    for (i = 0; i < numberOfData; ++i) {
      file << this->m_data[i];
      if (i < (numberOfData - 1))
        file << ",";
    }
  }

  file << "]" << std::endl;
  file << "\t\t\t}" << std::endl;
}

/**
 * Save footer
 */
void ThreeJS::saveFooter(std::ofstream &file) const {
  file << "\t\t}" << std::endl;
  file << "\t}" << std::endl;
  file << "}" << std::endl;
}

/**
 * Write part file
 * @param fileName File name
 * @param type Type
 * @param numberOfVolumes Number of volumes
 * @param numberOfSurfaces Number of surfaces
 * @param numberOfEdges Number of edges
 */
bool ThreeJS::writePartFile(const std::string &fileName,
                            const std::string &type, const uint numberOfVolumes,
                            const uint numberOfSurfaces,
                            const uint numberOfEdges) const {
  uint i;

  std::ofstream file(fileName, std::ios::out | std::ios::trunc);
  if (!file) {
    Logger::ERROR("Unable to open " + fileName);
    return false;
  }

  file << "{" << std::endl;
  file << "\t\"type\": \"" << type << "\"," << std::endl;
  file << "\t\"uuid\": \"" << generateUUID() << "\"," << std::endl;
  file << "\t\"solids\": [" << std::endl;

  for (i = 0; i < numberOfVolumes; ++i) {
    file << "\t\t{" << std::endl;
    file << "\t\t\t\"name\": \"" << SOLID << (i + 1) << "\"," << std::endl;
    file << "\t\t\t\"path\": \"" << SOLID << (i + 1) << ".json\"," << std::endl;
    file << "\t\t\t\"number\": \"" << (i + 1) << "\"" << std::endl;
    if (i < numberOfVolumes - 1)
      file << "\t\t}," << std::endl;
    else
      file << "\t\t}" << std::endl;
  }

  file << "\t]," << std::endl;
  file << "\t\"faces\": [" << std::endl;

  for (i = 0; i < numberOfSurfaces; ++i) {
    file << "\t\t{" << std::endl;
    file << "\t\t\t\"name\": \"" << FACE << (i + 1) << "\"," << std::endl;
    file << "\t\t\t\"path\": \"" << FACE << (i + 1) << ".json\"," << std::endl;
    file << "\t\t\t\"number\": \"" << (i + 1) << "\"" << std::endl;
    if (i < numberOfSurfaces - 1)
      file << "\t\t}," << std::endl;
    else
      file << "\t\t}" << std::endl;
  }

  if (!numberOfEdges)
    file << "\t]" << std::endl;
  else {
    file << "\t]," << std::endl;
    file << "\t\"edges\": [" << std::endl;

    for (i = 0; i < numberOfEdges; ++i) {
      file << "\t\t{" << std::endl;
      file << "\t\t\t\"name\": \"" << EDGE << (i + 1) << "\"," << std::endl;
      file << "\t\t\t\"path\": \"" << EDGE << (i + 1) << ".json\","
           << std::endl;
      file << "\t\t\t\"number\": \"" << (i + 1) << "\"" << std::endl;
      if (i < numberOfEdges - 1)
        file << "\t\t}," << std::endl;
      else
        file << "\t\t}" << std::endl;
    }

    file << "\t]" << std::endl;
  }

  file << "}" << std::endl;

  return true;
}

/**
 * Generate UUID
 * @returns UUID
 */
std::string ThreeJS::generateUUID() const {
  std::stringstream ss;
  for (uint i = 0; i < UUID_LENGHT; i++) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> dist(0, 255);
    const uint rc = dist(generator);
    std::stringstream hexstream;
    hexstream << std::hex << rc;
    auto hex = hexstream.str();
    ss << (hex.length() < 2 ? '0' + hex : hex);
  }
  return ss.str();
}
