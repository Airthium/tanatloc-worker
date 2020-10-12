#ifndef _THREEJS_
#define _THREEJS_

#include <string>

typedef unsigned int uint;

const std::string FACE = "face_";
const std::string SOLID = "solid_";
const std::string EDGE = "edge_";
const uint UUID_LENGHT = 16;

class ThreeJS {
private:
  // Dimension
  uint m_dimension = 3;

  // Number of vertices
  uint m_numberOfVertices = 0;
  // Number of normals
  uint m_numberOfNormals = 0;
  // Number of indices
  uint m_numberOfIndices = 0;
  // Number of colors
  uint m_numberOfColors = 0;

  // Label
  uint m_label = 0;
  // Vertices
  float *m_vertices = nullptr;
  // Normals
  float *m_normals = nullptr;
  // Indices
  uint *m_indices = nullptr;
  // Colors
  float **m_colors = nullptr;

  // Generate UUID
  std::string generateUUID() const;

public:
  // Constructor
  ThreeJS();
  ThreeJS(float *, const uint);
  ThreeJS(const double *, const uint);
  ThreeJS(float *, const uint, float *, const uint);
  ThreeJS(float *, const uint, float *, const uint, uint *, const uint);
  // Destructor
  ~ThreeJS();

  // Set label
  void setLabel(uint);
  // Set vertices
  void setVertices(float *, const uint);
  // Set normals
  void setNormals(float *, const uint);
  // Set indices
  void setIndices(uint *, const uint);
  // Set colors
  void setColors(float **, const uint);

  // Saver
  bool save(const std::string &) const;

  // Write part file
  bool writePartFile(const std::string &, const std::string &, const uint,
                     const uint, const uint numberOfEdges = 0) const;
};

#endif
