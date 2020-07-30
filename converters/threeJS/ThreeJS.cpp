#include "ThreeJS.hpp"

#include <fstream>
#include <iostream>
#include <random>
#include <sstream>

/**
 * Constructor
 */
ThreeJS::ThreeJS()
    : dimension(3), numberOfVertices(0), numberOfNormals(0), numberOfIndices(0),
      numberOfColors(0), minBb(0.), maxBb(0.), label(0), vertices(nullptr),
      normals(nullptr), indices(nullptr), colors(nullptr) {}

/**
 * Copy Constructor
 */
ThreeJS::ThreeJS(const ThreeJS &)
    : dimension(3), numberOfVertices(0), numberOfNormals(0), numberOfIndices(0),
      numberOfColors(0), minBb(0.), maxBb(0.), label(0), vertices(nullptr),
      normals(nullptr), indices(nullptr), colors(nullptr) {
  std::cerr << "Copy constructore not defined" << std::endl;
  throw std::string("Copy constructore not defined");
}

/**
 * Constructor
 * @param vertices Vertices (float)
 * @param numberOfVertices Number of vertices
 */
ThreeJS::ThreeJS(float *vertices, const uint numberOfVertices)
    : dimension(3), numberOfVertices(numberOfVertices), numberOfNormals(0),
      numberOfIndices(0), numberOfColors(0), minBb(0.), maxBb(0.), label(0),
      normals(nullptr), indices(nullptr), colors(nullptr) {
  this->vertices = new float[this->numberOfVertices];
  std::copy(vertices, vertices + numberOfVertices, this->vertices);
}

/**
 * Constructor
 * @param vertices Vertices (double)
 * @param numberOfVertices Number of vertices
 */
ThreeJS::ThreeJS(double *vertices, const uint numberOfVertices)
    : dimension(3), numberOfVertices(numberOfVertices), numberOfNormals(0),
      numberOfIndices(0), numberOfColors(0), minBb(0.), maxBb(0.), label(0),
      normals(nullptr), indices(nullptr), colors(nullptr) {
  uint i;
  this->vertices = new float[this->numberOfVertices];
  for (i = 0; i < numberOfVertices; ++i) {
    this->vertices[i] = (float)vertices[i];
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
    : dimension(3), numberOfVertices(numberOfVertices),
      numberOfNormals(numberOfNormals), numberOfColors(0), numberOfIndices(0),
      minBb(0.), maxBb(0.), label(0), indices(nullptr), colors(nullptr) {
  this->vertices = new float[this->numberOfVertices];
  std::copy(vertices, vertices + numberOfVertices, this->vertices);
  this->normals = new float[this->numberOfNormals];
  std::copy(normals, normals + numberOfNormals, this->normals);
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
    : dimension(3), numberOfVertices(numberOfVertices),
      numberOfNormals(numberOfNormals), numberOfIndices(numberOfIndices),
      numberOfColors(0), minBb(0.), maxBb(0.), label(0), colors(nullptr) {
  this->vertices = new float[this->numberOfVertices];
  std::copy(vertices, vertices + numberOfVertices, this->vertices);
  this->normals = new float[this->numberOfNormals];
  std::copy(normals, normals + numberOfNormals, this->normals);
  this->indices = new uint[this->numberOfIndices];
  std::copy(indices, indices + numberOfIndices, this->indices);
}

/**
 * Destructor
 */
ThreeJS::~ThreeJS() {
  if (this->vertices) {
    delete[] this->vertices;
    this->vertices = nullptr;
  }
  if (this->normals) {
    delete[] this->normals;
    this->normals = nullptr;
  }
  if (this->indices) {
    delete[] this->indices;
    this->indices = nullptr;
  }
  if (this->colors) {
    for (uint i = 0; i < this->numberOfColors; ++i) {
      if (this->colors[i]) {
        delete[] this->colors[i];
        this->colors[i] = nullptr;
      }
    }
    delete[] this->colors;
    this->colors = nullptr;
  }
  this->dimension = 0;
  this->numberOfVertices = 0;
  this->numberOfNormals = 0;
  this->numberOfIndices = 0;
}

/**
 * Set min & max
 */
void ThreeJS::setMinMax(double min, double max) {
  this->minBb = min;
  this->maxBb = max;
}

/**
 * Set label
 * @param number Label
 */
void ThreeJS::setLabel(uint number) { this->label = number; }

/**
 * Set vertices
 * @param vertices Vertices
 * @param size Size
 */
void ThreeJS::setVertices(float *vertices, const uint size) {
  if (this->vertices) {
    delete[] this->vertices;
  }

  this->numberOfVertices = size;
  this->vertices = new float[this->numberOfVertices];
  std::copy(vertices, vertices + size, this->vertices);
}

/**
 * Set normals
 * @param normals Normals
 * @param size Size
 */
void ThreeJS::setNormals(float *normals, const uint size) {
  if (this->normals) {
    delete[] this->normals;
  }

  this->numberOfNormals = size;
  this->normals = new float[this->numberOfNormals];
  std::copy(normals, normals + size, this->normals);
}

/**
 * Set indices
 * @param indices Indices
 * @param size Size
 */
void ThreeJS::setIndices(uint *indices, const uint size) {
  if (!this->indices) {
    delete[] this->indices;
  }

  this->numberOfIndices = size;
  this->indices = new uint[this->numberOfIndices];
  std::copy(indices, indices + size, this->indices);
}

void ThreeJS::setColors(float **colors, const uint size) {
  uint i;
  if (!this->colors) {
    for (i = 0; i < this->numberOfColors; ++i) {
      if (this->colors[i]) {
        delete[] this->colors[i];
        this->colors[i] = nullptr;
      }
    }
    delete[] this->colors;
  }

  this->numberOfColors = size;
  this->colors = new float *[this->numberOfColors];
  for (i = 0; i < this->numberOfColors; ++i) {
    this->colors[i] = new float[3];
    std::copy(colors[i], colors[i] + 3, this->colors[i]);
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
    std::cerr << "Unable to open " << fileName << std::endl;
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
  file << "\t\"minSize\": \"" << this->minBb << "\"," << std::endl;
  file << "\t\"maxSize\": \"" << this->maxBb << "\"," << std::endl;
  if (this->label)
    file << "\t\"label\": \"" << this->label << "\"," << std::endl;
  file << "\t\"type\": \"BufferGeometry\"," << std::endl;
  file << "\t\"data\": {" << std::endl;
  file << "\t\t\"attributes\": {" << std::endl;

  // Vertices
  if (this->vertices) {
    file << "\t\t\t\"position\": {" << std::endl;
    file << "\t\t\t\t\"itemSize\": " << this->dimension << "," << std::endl;
    file << "\t\t\t\t\"type\": \"Float32Array\"," << std::endl;
    file << "\t\t\t\t\"array\": [";

    if (this->indices) {
      for (i = 0; i < this->numberOfIndices; ++i) {
        file << this->vertices[3 * this->indices[i] + 0] << ","
             << this->vertices[3 * this->indices[i] + 1] << ","
             << this->vertices[3 * this->indices[i] + 2];
        if (i < (this->numberOfIndices - 1))
          file << ",";
      }
    } else {
      for (i = 0; i < this->numberOfVertices; ++i) {
        file << this->vertices[i];
        if (i < (this->numberOfVertices - 1))
          file << ",";
      }
    }

    file << "]" << std::endl;
    file << "\t\t\t}";

    if (this->normals || this->colors)
      file << "," << std::endl;
    else
      file << std::endl;
  }

  // Normals
  if (this->normals) {
    file << "\t\t\t\"normal\": {" << std::endl;
    file << "\t\t\t\t\"itemSize\": " << this->dimension << "," << std::endl;
    file << "\t\t\t\t\"type\": \"Float32Array\"," << std::endl;
    file << "\t\t\t\t\"array\": [";

    if (this->indices) {
      for (i = 0; i < this->numberOfIndices; ++i) {
        file << this->normals[3 * this->indices[i] + 0] << ","
             << this->normals[3 * this->indices[i] + 1] << ","
             << this->normals[3 * this->indices[i] + 2];
        if (i < (this->numberOfIndices - 1))
          file << ",";
      }
    } else {
      for (i = 0; i < this->numberOfNormals; ++i) {
        file << this->normals[i];
        if (i < (this->numberOfNormals - 1))
          file << ",";
      }
    }

    file << "]" << std::endl;
    file << "\t\t\t}";

    if (this->colors)
      file << "," << std::endl;
    else
      file << std::endl;
  }

  // Colors
  if (this->colors) {
    file << "\t\t\t\"color\": {" << std::endl;
    file << "\t\t\t\t\"itemSize\": " << this->dimension << "," << std::endl;
    file << "\t\t\t\t\"type\": \"Float32Array\"," << std::endl;
    file << "\t\t\t\t\"array\": [";

    for (i = 0; i < this->numberOfColors; ++i) {
      file << this->colors[i][0] << "," << this->colors[i][1] << ","
           << this->colors[i][2];
      if (i < (this->numberOfColors - 1))
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
    std::cerr << "Unable to open " << fileName << std::endl;
    return false;
  }

  file << "{" << std::endl;
  file << "\t\"type\": \"" << type << "\"," << std::endl;
  file << "\t\"solids\": [" << std::endl;

  for (i = 0; i < numberOfVolumes; ++i) {
    file << "\t\t{" << std::endl;
    file << "\t\t\t\"name\": \"" << SOLID << (i+1) << "\"," << std::endl;
    file << "\t\t\t\"path\": \"" << SOLID << (i+1) << ".json\"," << std::endl;
    file << "\t\t\t\"number\": \"" << (i+1) << "\"" << std::endl;
    if (i < numberOfVolumes - 1)
      file << "\t\t}," << std::endl;
    else
      file << "\t\t}" << std::endl;
  }

  file << "\t]," << std::endl;
  file << "\t\"faces\": [" << std::endl;

  for (i = 0; i < numberOfSurfaces; ++i) {
    file << "\t\t{" << std::endl;
    file << "\t\t\t\"name\": \"" << FACE << (i+1) << "\"," << std::endl;
    file << "\t\t\t\"path\": \"" << FACE << (i+1) << ".json\"," << std::endl;
    file << "\t\t\t\"number\": \"" << (i+1) << "\"" << std::endl;
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
      file << "\t\t\t\"name\": \"" << EDGE << (i+1) << "\"," << std::endl;
      file << "\t\t\t\"path\": \"" << EDGE << (i+1) << ".json\"," << std::endl;
      file << "\t\t\t\"number\": \"" << (i+1) << "\"" << std::endl;
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
 * Operator
 */
ThreeJS &ThreeJS::operator=(const ThreeJS &) {
  this->dimension = 3;
  this->numberOfVertices = 0;
  this->numberOfNormals = 0;
  this->numberOfIndices = 0;
  this->numberOfColors = 0;
  this->minBb = 0.;
  this->maxBb = 0.;
  this->label = 0;
  this->vertices = nullptr;
  this->normals = nullptr;
  this->indices = nullptr;
  this->colors = nullptr;
  std::cerr << "Copy constructore not defined" << std::endl;
  throw std::string("Equality operator not defined");
  return *this;
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
