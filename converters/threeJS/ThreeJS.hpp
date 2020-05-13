#ifndef _THREEJS_
#define _THREEJS_

#include <string>

typedef unsigned int uint;

#define FACE "face_"
#define SOLID "solid_"
#define EDGE "edge_"
#define UUID_LENGHT (uint)16

class ThreeJS {
private:
  // Dimension
  uint dimension;

  // Number of vertices
  uint numberOfVertices;
  // Number of normals
  uint numberOfNormals;
  // Number of indices
  uint numberOfIndices;
  // Number of colors
  uint numberOfColors;

  // Min Bb
  double minBb;
  // Max Bb
  double maxBb;
  //Label
  uint label;
  // Vertices
  float *vertices;
  // Normals
  float *normals;
  // Indices
  uint *indices;
  // Colors
  float **colors;

  // Generate UUID
  std::string generateUUID() const;
public:
  // Constructor
  ThreeJS();
  ThreeJS(const ThreeJS&);
  ThreeJS(float*, const uint);
  ThreeJS(double*, const uint);
  ThreeJS(float*, const uint, float*, const uint);
  ThreeJS(float*, const uint, float*, const uint, uint*, const uint);
  // Destructor
  ~ThreeJS();

  // Set min & max
  void setMinMax(double, double);
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
  bool save(const std::string&) const;

  // Write part file
  bool writePartFile(const std::string&, const std::string&, const uint, const uint, const uint numberOfEdges=0) const;

  // Operator
  ThreeJS &operator= (const ThreeJS&);
};

#endif
