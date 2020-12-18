#ifndef _THREEJS_
#define _THREEJS_

#include <string>
#include <vector>

using uint = unsigned int;

const std::string FACE = "face_";
const std::string SOLID = "solid_";
const std::string EDGE = "edge_";
const uint UUID_LENGHT = 16;

struct Color {
  float red = 0.;
  float green = 0.;
  float blue = 0.;
};

class ThreeJS {
private:
  // Dimension
  uint m_dimension = 3;

  // Name
  std::string m_name = "";
  // Label
  uint m_label = 0;
  // Vertices
  std::vector<float> m_vertices = std::vector<float>();
  // Normals
  std::vector<float> m_normals = std::vector<float>();
  // Indices
  std::vector<uint> m_indices = std::vector<uint>();
  // Colors
  std::vector<Color> m_colors;
  // Data
  std::vector<float> m_data = std::vector<float>();

  // Generate UUID
  std::string generateUUID() const;

public:
  // Constructor
  ThreeJS();
  explicit ThreeJS(const std::vector<float> &);
  ThreeJS(const std::vector<float> &, const std::vector<float> &);
  ThreeJS(const std::vector<float> &, const std::vector<float> &,
          const std::vector<uint> &);

  // Set name
  void setName(const std::string &);
  // Set label
  void setLabel(uint);
  // Set vertices
  void setVertices(const std::vector<float> &);
  // Set normals
  void setNormals(const std::vector<float> &);
  // Set indices
  void setIndices(const std::vector<uint> &);
  // Set colors
  void setColors(const std::vector<Color> &);
  // Set data
  void setData(const std::vector<float> &);

  // Saver
  bool save(const std::string &) const;
  void saveHeader(std::ofstream &) const;
  void saveVertices(std::ofstream &) const;
  void saveNormals(std::ofstream &) const;
  void saveColors(std::ofstream &) const;
  void saveData(std::ofstream &) const;
  void saveFooter(std::ofstream &) const;

  // Write part file
  bool writePartFile(const std::string &, const std::string &, const uint,
                     const uint, const uint numberOfEdges = 0) const;
};

#endif
