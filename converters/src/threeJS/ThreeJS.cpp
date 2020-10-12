#include "ThreeJS.hpp"

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
 * @param numberOfVertices Number of vertices
 */
ThreeJS::ThreeJS(float *vertices, const uint numberOfVertices)
    : m_numberOfVertices(numberOfVertices) {
  this->m_vertices = new float[numberOfVertices];
  std::copy(vertices, vertices + numberOfVertices, this->m_vertices);
}

/**
 * Constructor
 * @param vertices Vertices (double)
 * @param numberOfVertices Number of vertices
 */
ThreeJS::ThreeJS(double *vertices, const uint numberOfVertices)
    : m_numberOfVertices(numberOfVertices) {
  uint i;
  this->m_vertices = new float[numberOfVertices];
  for (i = 0; i < numberOfVertices; ++i) {
    this->m_vertices[i] = (float)vertices[i];
  }
}

/**
 * Constructor
 * @param vertices Vertices
 * @param numberOfVertices Number of vertices
 * @param normals Normals
 * @param numberOfNormals Number of normals
 */
ThreeJS::ThreeJS(float *vertices, const uint numberOfVertices, float *normals,
                 const uint numberOfNormals)
    : m_numberOfVertices(numberOfVertices), m_numberOfNormals(numberOfNormals) {
  this->m_vertices = new float[this->m_numberOfVertices];
  std::copy(vertices, vertices + numberOfVertices, this->m_vertices);
  this->m_normals = new float[this->m_numberOfNormals];
  std::copy(normals, normals + numberOfNormals, this->m_normals);
}

/**
 * Constructor
 * @param vertices Vertices
 * @param numberOfVertices Number of vertices
 * @param normals Normals
 * @param numberOfNormals Number of normals
 * @param indices Indices
 * @param numberOfIndices Number of indices
 */
ThreeJS::ThreeJS(float *vertices, const uint numberOfVertices, float *normals,
                 const uint numberOfNormals, uint *indices,
                 const uint numberOfIndices)
    : m_numberOfVertices(numberOfVertices), m_numberOfNormals(numberOfNormals),
      m_numberOfIndices(numberOfIndices) {
  this->m_vertices = new float[this->m_numberOfVertices];
  std::copy(vertices, vertices + numberOfVertices, this->m_vertices);
  this->m_normals = new float[this->m_numberOfNormals];
  std::copy(normals, normals + numberOfNormals, this->m_normals);
  this->m_indices = new uint[this->m_numberOfIndices];
  std::copy(indices, indices + numberOfIndices, this->m_indices);
}

/**
 * Destructor
 */
ThreeJS::~ThreeJS() {
  if (this->m_vertices) {
    delete[] this->m_vertices;
    this->m_vertices = nullptr;
  }
  if (this->m_normals) {
    delete[] this->m_normals;
    this->m_normals = nullptr;
  }
  if (this->m_indices) {
    delete[] this->m_indices;
    this->m_indices = nullptr;
  }
  if (this->m_colors) {
    for (uint i = 0; i < this->m_numberOfColors; ++i) {
      if (this->m_colors[i]) {
        delete[] this->m_colors[i];
        this->m_colors[i] = nullptr;
      }
    }
    delete[] this->m_colors;
    this->m_colors = nullptr;
  }
  this->m_dimension = 0;
  this->m_numberOfVertices = 0;
  this->m_numberOfNormals = 0;
  this->m_numberOfIndices = 0;
}

/**
 * Set min & max
 */
void ThreeJS::setMinMax(double min, double max) {
  this->m_minBb = min;
  this->m_maxBb = max;
}

/**
 * Set label
 * @param number Label
 */
void ThreeJS::setLabel(uint number) { this->m_label = number; }

/**
 * Set vertices
 * @param vertices Vertices
 * @param size Size
 */
void ThreeJS::setVertices(float *vertices, const uint size) {
  if (this->m_vertices) {
    delete[] this->m_vertices;
  }

  this->m_numberOfVertices = size;
  this->m_vertices = new float[this->m_numberOfVertices];
  std::copy(vertices, vertices + size, this->m_vertices);
}

/**
 * Set normals
 * @param normals Normals
 * @param size Size
 */
void ThreeJS::setNormals(float *normals, const uint size) {
  if (this->m_normals) {
    delete[] this->m_normals;
  }

  this->m_numberOfNormals = size;
  this->m_normals = new float[this->m_numberOfNormals];
  std::copy(normals, normals + size, this->m_normals);
}

/**
 * Set indices
 * @param indices Indices
 * @param size Size
 */
void ThreeJS::setIndices(uint *indices, const uint size) {
  if (!this->m_indices) {
    delete[] this->m_indices;
  }

  this->m_numberOfIndices = size;
  this->m_indices = new uint[this->m_numberOfIndices];
  std::copy(indices, indices + size, this->m_indices);
}

void ThreeJS::setColors(float **colors, const uint size) {
  uint i;
  if (!this->m_colors) {
    for (i = 0; i < this->m_numberOfColors; ++i) {
      if (this->m_colors[i]) {
        delete[] this->m_colors[i];
        this->m_colors[i] = nullptr;
      }
    }
    delete[] this->m_colors;
  }

  this->m_numberOfColors = size;
  this->m_colors = new float *[this->m_numberOfColors];
  for (i = 0; i < this->m_numberOfColors; ++i) {
    this->m_colors[i] = new float[3];
    std::copy(colors[i], colors[i] + 3, this->m_colors[i]);
  }
}

/**
 * Saver
 * @param {string} fileName File name
 * @returns {boolean} Save status
 */
bool ThreeJS::save(const std::string &fileName) const {
  uint i;

  std::ofstream file(fileName, std::ios::out | std::ios::trunc);
  if (!file) {
    Logger::ERROR("Unable to open " + fileName);
    return false;
  }

  // Headers
  file << "{" << std::endl;
  file << "\t\"metadata\": {" << std::endl;
  file << "\t\t\"version\": 4.4," << std::endl;
  file << "\t\t\"type\": \"BufferGeometry\"," << std::endl;
  file << "\t\t\"generator\": \"Tanatloc\"" << std::endl;
  file << "\t}," << std::endl;
  file << "\t\"uuid\": \"" << this->generateUUID() << "\"," << std::endl;
  if (this->m_label)
    file << "\t\"label\": \"" << this->m_label << "\"," << std::endl;
  file << "\t\"type\": \"BufferGeometry\"," << std::endl;
  file << "\t\"data\": {" << std::endl;
  file << "\t\t\"attributes\": {" << std::endl;

  // Vertices
  if (this->m_vertices) {
    file << "\t\t\t\"position\": {" << std::endl;
    file << "\t\t\t\t\"itemSize\": " << this->m_dimension << "," << std::endl;
    file << "\t\t\t\t\"type\": \"Float32Array\"," << std::endl;
    file << "\t\t\t\t\"array\": [";

    if (this->m_indices) {
      for (i = 0; i < this->m_numberOfIndices; ++i) {
        file << this->m_vertices[3 * this->m_indices[i] + 0] << ","
             << this->m_vertices[3 * this->m_indices[i] + 1] << ","
             << this->m_vertices[3 * this->m_indices[i] + 2];
        if (i < (this->m_numberOfIndices - 1))
          file << ",";
      }
    } else {
      for (i = 0; i < this->m_numberOfVertices; ++i) {
        file << this->m_vertices[i];
        if (i < (this->m_numberOfVertices - 1))
          file << ",";
      }
    }

    file << "]" << std::endl;
    file << "\t\t\t}";

    if (this->m_normals || this->m_colors)
      file << "," << std::endl;
    else
      file << std::endl;
  }

  // Normals
  if (this->m_normals) {
    file << "\t\t\t\"normal\": {" << std::endl;
    file << "\t\t\t\t\"itemSize\": " << this->m_dimension << "," << std::endl;
    file << "\t\t\t\t\"type\": \"Float32Array\"," << std::endl;
    file << "\t\t\t\t\"array\": [";

    if (this->m_indices) {
      for (i = 0; i < this->m_numberOfIndices; ++i) {
        file << this->m_normals[3 * this->m_indices[i] + 0] << ","
             << this->m_normals[3 * this->m_indices[i] + 1] << ","
             << this->m_normals[3 * this->m_indices[i] + 2];
        if (i < (this->m_numberOfIndices - 1))
          file << ",";
      }
    } else {
      for (i = 0; i < this->m_numberOfNormals; ++i) {
        file << this->m_normals[i];
        if (i < (this->m_numberOfNormals - 1))
          file << ",";
      }
    }

    file << "]" << std::endl;
    file << "\t\t\t}";

    if (this->m_colors)
      file << "," << std::endl;
    else
      file << std::endl;
  }

  // Colors
  if (this->m_colors) {
    file << "\t\t\t\"color\": {" << std::endl;
    file << "\t\t\t\t\"itemSize\": " << this->m_dimension << "," << std::endl;
    file << "\t\t\t\t\"type\": \"Float32Array\"," << std::endl;
    file << "\t\t\t\t\"array\": [";

    for (i = 0; i < this->m_numberOfColors; ++i) {
      file << this->m_colors[i][0] << "," << this->m_colors[i][1] << ","
           << this->m_colors[i][2];
      if (i < (this->m_numberOfColors - 1))
        file << ",";
    }

    file << "]" << std::endl;
    file << "\t\t\t}" << std::endl;
  }

  // Footer
  file << "\t\t}" << std::endl;
  file << "\t}" << std::endl;
  file << "}" << std::endl;

  return true;
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
